package vsnet.simulations;

import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
//import inet.visualizer.integrated.Integ		ratedCanvasVisualizer;
import inet.common.misc.NetAnimTrace;
import inet.common.scenario.ScenarioManager;
import ned.DatarateChannel;


network scenario
{
    parameters:
        double per = default(0);
    types:
        channel C extends DatarateChannel
        {
            datarate = 25Mbps;//0.8Mbps
            delay = 121ms; //0.565us
            per = per;
        }
    submodules:
        client: StandardHost {
            parameters:
                @display("p=50,100");
        }
        server: StandardHost {
            parameters:
                @display("p=350,100;i=device/server");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=58,19;is=s");
        }
//        visualizer: IntegratedCanvasVisualizer {
//            @display("p=157,19;is=s");
//        }
    connections:
        client.pppg++ <--> C <--> server.pppg++;
}
