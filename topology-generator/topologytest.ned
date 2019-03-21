import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import ned.DatarateChannel;
import inet.node.inet.Router;

network topologytest
{
    parameters:
    
        // Channel C0 parameters (to be redefined in .ini file)
        double C0_datarate @unit(bps) = default(2Mbps);
        double C0_delay @unit(s) = default(0.1us);
        double C0_per = default(0);
        double C0_ber = default(0);

        // Channel C1 parameters (to be redefined in .ini file)
        double C1_datarate @unit(bps) = default(2Mbps);
        double C1_delay @unit(s) = default(0.1us);
        double C1_per = default(0);
        double C1_ber = default(0);
    
    types:
    
        channel C0 extends DatarateChannel
        {
            datarate = C0_datarate;
            delay = C0_delay;
            per = C0_per;
            ber = C0_ber;
        }

        channel C1 extends DatarateChannel
        {
            datarate = C1_datarate;
            delay = C1_delay;
            per = C1_per;
            ber = C1_ber;
        }
    
    submodules:
    
        client0:StandardHost{}

        client1:StandardHost{}

        client2:StandardHost{}

        client3:StandardHost{}

        client4:StandardHost{}

        client5:StandardHost{}

        client6:StandardHost{}

        client7:StandardHost{}

        client8:StandardHost{}

        client9:StandardHost{}

        client10:StandardHost{}

        client11:StandardHost{}

        client12:StandardHost{}

        client13:StandardHost{}

        client14:StandardHost{}

        client15:StandardHost{}

        client16:StandardHost{}

        client17:StandardHost{}

        client18:StandardHost{}

        client19:StandardHost{}

        client20:StandardHost{}

        client21:StandardHost{}

        client22:StandardHost{}

        client23:StandardHost{}

        client24:StandardHost{}

        client25:StandardHost{}

        client26:StandardHost{}

        client27:StandardHost{}

        client28:StandardHost{}

        client29:StandardHost{}

        client30:StandardHost{}

        client31:StandardHost{}

        client32:StandardHost{}

        client33:StandardHost{}

        client34:StandardHost{}

        client35:StandardHost{}

        client36:StandardHost{}

        client37:StandardHost{}

        client38:StandardHost{}

        client39:StandardHost{}

        client40:StandardHost{}

        client41:StandardHost{}

        client42:StandardHost{}

        client43:StandardHost{}

        client44:StandardHost{}

        client45:StandardHost{}

        client46:StandardHost{}

        client47:StandardHost{}

        client48:StandardHost{}

        client49:StandardHost{}

        client50:StandardHost{}

        client51:StandardHost{}

        client52:StandardHost{}

        client53:StandardHost{}

        client54:StandardHost{}

        client55:StandardHost{}

        client56:StandardHost{}

        client57:StandardHost{}

        client58:StandardHost{}

        client59:StandardHost{}

        client60:StandardHost{}

        client61:StandardHost{}

        client62:StandardHost{}

        client63:StandardHost{}

        client64:StandardHost{}

        client65:StandardHost{}

        client66:StandardHost{}

        client67:StandardHost{}

        client68:StandardHost{}

        client69:StandardHost{}

        client70:StandardHost{}

        client71:StandardHost{}

        client72:StandardHost{}

        client73:StandardHost{}

        client74:StandardHost{}

        client75:StandardHost{}

        client76:StandardHost{}

        client77:StandardHost{}

        client78:StandardHost{}

        client79:StandardHost{}

        client80:StandardHost{}

        client81:StandardHost{}

        client82:StandardHost{}

        client83:StandardHost{}

        client84:StandardHost{}

        client85:StandardHost{}

        client86:StandardHost{}

        client87:StandardHost{}

        client88:StandardHost{}

        client89:StandardHost{}

        client90:StandardHost{}

        client91:StandardHost{}

        client92:StandardHost{}

        client93:StandardHost{}

        client94:StandardHost{}

        client95:StandardHost{}

        client96:StandardHost{}

        client97:StandardHost{}

        client98:StandardHost{}

        client99:StandardHost{}

        router0:Router{}

        router1:Router{}

        server0:StandardHost{}

        configurator: Ipv4NetworkConfigurator {}
    
    connections:
    
        client0.pppg++ <--> C1 <--> router0.pppg++;

        client1.pppg++ <--> C1 <--> router0.pppg++;

        client2.pppg++ <--> C1 <--> router0.pppg++;

        client3.pppg++ <--> C1 <--> router0.pppg++;

        client4.pppg++ <--> C1 <--> router0.pppg++;

        client5.pppg++ <--> C1 <--> router0.pppg++;

        client6.pppg++ <--> C1 <--> router0.pppg++;

        client7.pppg++ <--> C1 <--> router0.pppg++;

        client8.pppg++ <--> C1 <--> router0.pppg++;

        client9.pppg++ <--> C1 <--> router0.pppg++;

        client10.pppg++ <--> C1 <--> router0.pppg++;

        client11.pppg++ <--> C1 <--> router0.pppg++;

        client12.pppg++ <--> C1 <--> router0.pppg++;

        client13.pppg++ <--> C1 <--> router0.pppg++;

        client14.pppg++ <--> C1 <--> router0.pppg++;

        client15.pppg++ <--> C1 <--> router0.pppg++;

        client16.pppg++ <--> C1 <--> router0.pppg++;

        client17.pppg++ <--> C1 <--> router0.pppg++;

        client18.pppg++ <--> C1 <--> router0.pppg++;

        client19.pppg++ <--> C1 <--> router0.pppg++;

        client20.pppg++ <--> C1 <--> router0.pppg++;

        client21.pppg++ <--> C1 <--> router0.pppg++;

        client22.pppg++ <--> C1 <--> router0.pppg++;

        client23.pppg++ <--> C1 <--> router0.pppg++;

        client24.pppg++ <--> C1 <--> router0.pppg++;

        client25.pppg++ <--> C1 <--> router0.pppg++;

        client26.pppg++ <--> C1 <--> router0.pppg++;

        client27.pppg++ <--> C1 <--> router0.pppg++;

        client28.pppg++ <--> C1 <--> router0.pppg++;

        client29.pppg++ <--> C1 <--> router0.pppg++;

        client30.pppg++ <--> C1 <--> router0.pppg++;

        client31.pppg++ <--> C1 <--> router0.pppg++;

        client32.pppg++ <--> C1 <--> router0.pppg++;

        client33.pppg++ <--> C1 <--> router0.pppg++;

        client34.pppg++ <--> C1 <--> router0.pppg++;

        client35.pppg++ <--> C1 <--> router0.pppg++;

        client36.pppg++ <--> C1 <--> router0.pppg++;

        client37.pppg++ <--> C1 <--> router0.pppg++;

        client38.pppg++ <--> C1 <--> router0.pppg++;

        client39.pppg++ <--> C1 <--> router0.pppg++;

        client40.pppg++ <--> C1 <--> router0.pppg++;

        client41.pppg++ <--> C1 <--> router0.pppg++;

        client42.pppg++ <--> C1 <--> router0.pppg++;

        client43.pppg++ <--> C1 <--> router0.pppg++;

        client44.pppg++ <--> C1 <--> router0.pppg++;

        client45.pppg++ <--> C1 <--> router0.pppg++;

        client46.pppg++ <--> C1 <--> router0.pppg++;

        client47.pppg++ <--> C1 <--> router0.pppg++;

        client48.pppg++ <--> C1 <--> router0.pppg++;

        client49.pppg++ <--> C1 <--> router0.pppg++;

        client50.pppg++ <--> C1 <--> router1.pppg++;

        client51.pppg++ <--> C1 <--> router1.pppg++;

        client52.pppg++ <--> C1 <--> router1.pppg++;

        client53.pppg++ <--> C1 <--> router1.pppg++;

        client54.pppg++ <--> C1 <--> router1.pppg++;

        client55.pppg++ <--> C1 <--> router1.pppg++;

        client56.pppg++ <--> C1 <--> router1.pppg++;

        client57.pppg++ <--> C1 <--> router1.pppg++;

        client58.pppg++ <--> C1 <--> router1.pppg++;

        client59.pppg++ <--> C1 <--> router1.pppg++;

        client60.pppg++ <--> C1 <--> router1.pppg++;

        client61.pppg++ <--> C1 <--> router1.pppg++;

        client62.pppg++ <--> C1 <--> router1.pppg++;

        client63.pppg++ <--> C1 <--> router1.pppg++;

        client64.pppg++ <--> C1 <--> router1.pppg++;

        client65.pppg++ <--> C1 <--> router1.pppg++;

        client66.pppg++ <--> C1 <--> router1.pppg++;

        client67.pppg++ <--> C1 <--> router1.pppg++;

        client68.pppg++ <--> C1 <--> router1.pppg++;

        client69.pppg++ <--> C1 <--> router1.pppg++;

        client70.pppg++ <--> C1 <--> router1.pppg++;

        client71.pppg++ <--> C1 <--> router1.pppg++;

        client72.pppg++ <--> C1 <--> router1.pppg++;

        client73.pppg++ <--> C1 <--> router1.pppg++;

        client74.pppg++ <--> C1 <--> router1.pppg++;

        client75.pppg++ <--> C1 <--> router1.pppg++;

        client76.pppg++ <--> C1 <--> router1.pppg++;

        client77.pppg++ <--> C1 <--> router1.pppg++;

        client78.pppg++ <--> C1 <--> router1.pppg++;

        client79.pppg++ <--> C1 <--> router1.pppg++;

        client80.pppg++ <--> C1 <--> router1.pppg++;

        client81.pppg++ <--> C1 <--> router1.pppg++;

        client82.pppg++ <--> C1 <--> router1.pppg++;

        client83.pppg++ <--> C1 <--> router1.pppg++;

        client84.pppg++ <--> C1 <--> router1.pppg++;

        client85.pppg++ <--> C1 <--> router1.pppg++;

        client86.pppg++ <--> C1 <--> router1.pppg++;

        client87.pppg++ <--> C1 <--> router1.pppg++;

        client88.pppg++ <--> C1 <--> router1.pppg++;

        client89.pppg++ <--> C1 <--> router1.pppg++;

        client90.pppg++ <--> C1 <--> router1.pppg++;

        client91.pppg++ <--> C1 <--> router1.pppg++;

        client92.pppg++ <--> C1 <--> router1.pppg++;

        client93.pppg++ <--> C1 <--> router1.pppg++;

        client94.pppg++ <--> C1 <--> router1.pppg++;

        client95.pppg++ <--> C1 <--> router1.pppg++;

        client96.pppg++ <--> C1 <--> router1.pppg++;

        client97.pppg++ <--> C1 <--> router1.pppg++;

        client98.pppg++ <--> C1 <--> router1.pppg++;

        client99.pppg++ <--> C1 <--> router1.pppg++;

        router0.pppg++ <--> C0 <--> server0.pppg++;

        router1.pppg++ <--> C0 <--> server0.pppg++;
}
