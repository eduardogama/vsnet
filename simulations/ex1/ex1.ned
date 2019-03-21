//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package vsnet.simulations.ex1;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;

//
// TODO auto-generated type
//
network ex1
{
    @display("bgb=750,447");
    types:
        channel line1 extends DatarateChannel
        {
            parameters:
                delay = 100us;
                datarate = 100Mbps;
        }

        channel line2 extends DatarateChannel
        {
            parameters:
                delay = 10us;
                datarate = 100Mbps;
        }
    submodules:
        client: StandardHost {
            parameters:
                @display("p=44,225;i=device/laptop_l");
        }
        switch: EtherSwitch {
            parameters:
                @display("p=190,224;i=device/switch");
        }
        router1: Router {
            parameters:
                @display("p=334,227;i=abstract/router");
        }
        server: StandardHost {
            parameters:
                @display("p=628,227;i=device/server_l");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                config = xml("<config><interface hosts='*' address='10.0.x.x' netmask='255.255.x.x'/></config>");
                @display("p=44,19;i=block/cogwheel_s");
        }
        switch2: EtherSwitch {
            @display("p=473,224");
        }
    connections:
        client.ethg++ <--> line1 <--> switch.ethg++;
        router1.ethg++ <--> line1 <--> switch.ethg++;
        switch2.ethg++ <--> line2 <--> router1.ethg++;
        switch2.ethg++ <--> line1 <--> server.ethg++;
}
