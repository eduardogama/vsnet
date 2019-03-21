package vsnet.simulations;

import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network scenario
{
    parameters:
        double n;
        @display("bgb=400,200");
    submodules:
        client: StandardHost {
            parameters:
                @display("p=50,100");
            gates:
                pppg[1];
        }
        server: StandardHost {
            parameters:
                @display("p=350,100;i=device/server");
            gates:
                pppg[1];
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=199,34");
        }
    connections:
        client.pppg[0] <--> {  datarate = 0.8Mbps; delay = 0.565us; per = 0; ber = n; } <--> server.pppg[0];
}


