#!/usr/local/bin/python2.7
# encoding: utf-8
'''
TreeTopologyGenerator -- OMNET++ network topology file generator.

DASHTopologyGenerator is a script that generates a .NED file containing a
tree topology for network simulation OMNET++ and INET Framework.

@author:     Joaquin Navarro
        
@copyright:  2013 DIT-UPM.
        
@license:    TBD

@contact:    navarro@dit.upm.es
@deffield    updated: Updated
'''

import sys
import os

from argparse import ArgumentParser
from argparse import RawDescriptionHelpFormatter

__all__ = []
__version__ = 0.1
__date__ = '2013-09-04'
__updated__ = '2013-09-04'

DEBUG = 0
TESTRUN = 0
PROFILE = 0

class CLIError(Exception):
    '''Generic exception to raise and log different fatal errors.'''
    def __init__(self, msg):
        super(CLIError).__init__(type(self))
        self.msg = "E: %s" % msg
    def __str__(self):
        return self.msg
    def __unicode__(self):
        return self.msg
    


def main(argv=None): # IGNORE:C0111
    '''Command line options.'''
    
    if argv is None:
        argv = sys.argv
    else:
        sys.argv.extend(argv)

    program_name = os.path.basename(sys.argv[0])
    program_version = "v%s" % __version__
    program_build_date = str(__updated__)
    program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
    program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
    program_license = '''%s

  Created by Joaquín Navarro on %s.
  Copyright 2013 DIT-UPM. All rights reserved.
  
  Licensed under the Apache License 2.0
  http://www.apache.org/licenses/LICENSE-2.0
  
  Distributed on an "AS IS" basis without warranties
  or conditions of any kind, either express or implied.
  
  Execution example: python TreeTopologyGenerator.py topologytest 3 1 server StandardHost 2 router Router 50 client StandardHost

USAGE
''' % (program_shortdesc, str(__date__))


    ### Variables ###
    channelPrototype = """
        channel {0} extends DatarateChannel
        {{
            datarate = {0}_datarate;
            delay = {0}_delay;
            per = {0}_per;
            ber = {0}_ber;
        }}"""
    channelParameterPrototype = """
        // Channel {0} parameters (to be redefined in .ini file)
        double {0}_datarate @unit(bps) = default(2Mbps);
        double {0}_delay @unit(s) = default(0.1us);
        double {0}_per = default(0);
        double {0}_ber = default(0);"""
    subModulePrototype = """
        {0}{1}:{2}{{}}"""
    connectionPrototype = """
        {0}{1}.pppg++ <--> {2} <--> {3}{4}.pppg++;"""
    channelNamePrototype = 'C{0}'
        
    configuratorSubModule = """
        configurator: IPv4NetworkConfigurator {}"""
        
    scenarioFilePrototype = """package {0}

import inet.nodes.inet.StandardHost;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import ned.DatarateChannel;
import inet.nodes.inet.Router;

network {1}
{{
    parameters:
    {2}
    
    types:
    {3}
    
    submodules:
    {4}
    
    connections:
    {5}
}}
"""
    default_package = "tcp_extension.simulations"

    try:
        # Setup argument parser
        parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)
        parser.add_argument("-v", "--verbose", dest="verbose", action="count", help="set verbosity level [default: %(default)s]")
        parser.add_argument('-V', '--version', action='version', version=program_version_message)
        parser.add_argument('scenario', help="name of the scenario", type = str)
        parser.add_argument('nLevels', help="number of levels of the tree topology", type=int)
        parser.add_argument('treeConfig', help="tree configuration: for each tree level (and starting from the root) set a triplet <number of children of each node on the previous level> <node label> <node type>.", nargs='+')
        parser.add_argument('-p', '--package', help=".ned file package")

        
        # Process arguments
        args = parser.parse_args()
        verbose = args.verbose
        if verbose > 0:
            print("Verbose mode on")
        scenario = args.scenario
        treeConfig = args.treeConfig
        childrens = []
        labels = []
        types = []
        nLevels = args.nLevels
        package = args.package if args.package else default_package
        
        for i, item in enumerate(treeConfig):
            if (i % 3 == 0):
                childrens.append(int(item))
            elif (i % 3 == 1):
                labels.append(item)
            else:
                types.append(item)
        
        if (verbose > 0):
            print "Tree configuration:"
            print "\tchildren info: ", childrens
            print "\tlabel info: ", labels
            print "\ttypes info: ", types
            
        if (len(childrens) != nLevels or len(labels) != nLevels):
            raise CLIError("Wrong arguments. Example: 3 1 server StandardHost 2 router Router 50 client StandardHost. This creates a tree level topology with 1 server 2 routers and (2x50)=100 clients")
        
        
        subModules = []
        connections = []
        channelNames = []
        channelParameters = []
        channels = []
        
        nodesPerLevel = []
        for level, childCount in enumerate(childrens):
            if (level == 0):
                nodesPerLevel.append(childCount)
            else:
                nodesPerLevel.append(nodesPerLevel[level-1] * childCount);     
        
        #### Global dimension ####   OMNET++ makes a by default layout quite good. FUTURE WORK
        #height = 40 + (nodesPerLevel[-1] - 1) * 60
        #width = 40 + nLevels * 90
        
        #### Channels ####
        for level in range(nLevels-1):
            channelNames.append(channelNamePrototype.format(level))
            channelParameters.append(channelParameterPrototype.format(channelNames[level]))
            channels.append(channelPrototype.format(channelNames[level]))
            
        #### Nodes ####
        ## TO-DO: Set display parameters. OMNET++ makes a by default layout quite good. FUTURE WORK ##
        for level, childCount in enumerate(reversed(childrens)):
            level = len(childrens) - 1 - level
            if (verbose > 0):
                print "Level:", level, ". Number of childrens:", childCount
            for j in range(nodesPerLevel[level]):
                subModuleDefinition = subModulePrototype.format(labels[level], j, types[level])
                subModules.append(subModuleDefinition)
                if (verbose > 0):
                    print subModuleDefinition
                if (level == 0):
                    continue
                parentId = j / childCount
                connectionDefinition = connectionPrototype.format(labels[level], j, channelNames[level-1], labels[level-1], parentId)
                connections.append(connectionDefinition)
                if (verbose > 0):
                    print connectionDefinition
                
        subModules.append(configuratorSubModule)
        
        #### Write file ####
        nedFile = open(scenario + ".ned", 'w')
        nedFile.write(scenarioFilePrototype.format(package,
                                                   scenario,
                                                   '\n'.join(channelParameters),                                                   
                                                   '\n'.join(channels),                                                   
                                                   '\n'.join(subModules),
                                                   '\n'.join(connections)))
        
        print "\n.ned file generated successfully"
        return 0

    except Exception, e:
        if DEBUG or TESTRUN:
            raise(e)
        indent = len(program_name) * " "
        sys.stderr.write(program_name + ": " + repr(e) + "\n")
        sys.stderr.write(indent + "  for help use --help")
        return 2

if __name__ == "__main__":
    if DEBUG:
        sys.argv.append("-v")
    if TESTRUN:
        import doctest
        doctest.testmod()
    if PROFILE:
        import cProfile
        import pstats
        profile_filename = 'TreeTopologyGenerator_profile.txt'
        cProfile.run('main()', profile_filename)
        statsfile = open("profile_stats.txt", "wb")
        p = pstats.Stats(profile_filename, stream=statsfile)
        stats = p.strip_dirs().sort_stats('cumulative')
        stats.print_stats()
        statsfile.close()
        sys.exit(0)
    sys.exit(main())