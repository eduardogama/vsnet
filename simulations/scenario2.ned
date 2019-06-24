package vsnet.simulations;


import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import ned.DatarateChannel;
import inet.node.inet.Router;
import inet.common.misc.NetAnimTrace;
import inet.applications.httptools.configurator.HttpController;

network scenario2
{
    parameters:
        // Channel 1 parameters (to be redefined in .ini file)
        double C1_datarate @unit(bps) = default(2Mbps);
        double C1_delay @unit(s) = default(8ms);
        double C1_per = default(0);
        double C1_ber = default(0);

        // Channel 2 parameters (to be redefined in .ini file)
        double C2_datarate @unit(bps) = default(4Mbps);
        double C2_delay @unit(s) = default(48ms);
        double C2_per = default(0);
        double C2_ber = default(0);

        // Channel 3 parameters (to be redefined in .ini file)
        double C3_datarate @unit(bps) = default(4Mbps);
        double C3_delay @unit(s) = default(121ms);
        double C3_per = default(0);
        double C3_ber = default(0);

        @display("bgb=500,250");
    types:
        channel C1 extends DatarateChannel
        {
            datarate = C1_datarate;
            delay = C1_delay;
            per = C1_per;
            ber = C1_ber;
        }
        channel C2 extends DatarateChannel
        {
            datarate = C2_datarate;
            delay = C2_delay;
            per = C2_per;
            ber = C2_ber;
        }
        channel C3 extends DatarateChannel
        {
            datarate = C3_datarate;
            delay = C3_delay;
            per = C3_per;
            ber = C3_ber;
        }
    submodules:
        client1: StandardHost {
            parameters:
                @display("p=50,80");
        }
        server1: StandardHost {
            parameters:
                @display("p=400,120;i=device/server");
        }
        server2: StandardHost {
            parameters:
                @display("p=50,176;i=device/server");
        }
        router1: Router {
            parameters:
                @display("p=180,120");
        }
        router2: Router {
            parameters:
                @display("p=300,120");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=154,34");
        }
    connections:
        client1.pppg++ <--> C1 <--> router1.pppg++;
        server2.pppg++ <--> C1 <--> router1.pppg++;
        router1.pppg++ <--> C2 <--> router2.pppg++;
        router2.pppg++ <--> C3 <--> server1.pppg++;
}
