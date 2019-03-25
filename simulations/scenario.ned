package vsnet.simulations;

import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network scenario
{
    @display("bgb=386,180");
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
        visualizer: IntegratedCanvasVisualizer {
            @display("p=157,19;is=s");
        }
    connections:
        client.pppg++ <--> {  datarate = 0.8Mbps; delay = 0.565us; per = 0; } <--> server.pppg++;
}


