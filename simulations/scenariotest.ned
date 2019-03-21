package vsnet.simulations;

import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import ned.DatarateChannel;
import inet.node.inet.Router;

network scenariotest
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

        // Channel C2 parameters (to be redefined in .ini file)
        double C2_datarate @unit(bps) = default(2Mbps);
        double C2_delay @unit(s) = default(0.1us);
        double C2_per = default(0);
        double C2_ber = default(0);
    
    
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

        channel C2 extends DatarateChannel
        {
            datarate = C2_datarate;
            delay = C2_delay;
            per = C2_per;
            ber = C2_ber;
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

        client100:StandardHost{}

        client101:StandardHost{}

        client102:StandardHost{}

        client103:StandardHost{}

        client104:StandardHost{}

        client105:StandardHost{}

        client106:StandardHost{}

        client107:StandardHost{}

        client108:StandardHost{}

        client109:StandardHost{}

        client110:StandardHost{}

        client111:StandardHost{}

        client112:StandardHost{}

        client113:StandardHost{}

        client114:StandardHost{}

        client115:StandardHost{}

        client116:StandardHost{}

        client117:StandardHost{}

        client118:StandardHost{}

        client119:StandardHost{}

        client120:StandardHost{}

        client121:StandardHost{}

        client122:StandardHost{}

        client123:StandardHost{}

        client124:StandardHost{}

        client125:StandardHost{}

        client126:StandardHost{}

        client127:StandardHost{}

        client128:StandardHost{}

        client129:StandardHost{}

        client130:StandardHost{}

        client131:StandardHost{}

        client132:StandardHost{}

        client133:StandardHost{}

        client134:StandardHost{}

        client135:StandardHost{}

        client136:StandardHost{}

        client137:StandardHost{}

        client138:StandardHost{}

        client139:StandardHost{}

        client140:StandardHost{}

        client141:StandardHost{}

        client142:StandardHost{}

        client143:StandardHost{}

        client144:StandardHost{}

        client145:StandardHost{}

        client146:StandardHost{}

        client147:StandardHost{}

        client148:StandardHost{}

        client149:StandardHost{}

        client150:StandardHost{}

        client151:StandardHost{}

        client152:StandardHost{}

        client153:StandardHost{}

        client154:StandardHost{}

        client155:StandardHost{}

        client156:StandardHost{}

        client157:StandardHost{}

        client158:StandardHost{}

        client159:StandardHost{}

        client160:StandardHost{}

        client161:StandardHost{}

        client162:StandardHost{}

        client163:StandardHost{}

        client164:StandardHost{}

        client165:StandardHost{}

        client166:StandardHost{}

        client167:StandardHost{}

        client168:StandardHost{}

        client169:StandardHost{}

        client170:StandardHost{}

        client171:StandardHost{}

        client172:StandardHost{}

        client173:StandardHost{}

        client174:StandardHost{}

        client175:StandardHost{}

        client176:StandardHost{}

        client177:StandardHost{}

        client178:StandardHost{}

        client179:StandardHost{}

        client180:StandardHost{}

        client181:StandardHost{}

        client182:StandardHost{}

        client183:StandardHost{}

        client184:StandardHost{}

        client185:StandardHost{}

        client186:StandardHost{}

        client187:StandardHost{}

        client188:StandardHost{}

        client189:StandardHost{}

        client190:StandardHost{}

        client191:StandardHost{}

        client192:StandardHost{}

        client193:StandardHost{}

        client194:StandardHost{}

        client195:StandardHost{}

        client196:StandardHost{}

        client197:StandardHost{}

        client198:StandardHost{}

        client199:StandardHost{}

        routerB0:Router{}

        routerB1:Router{}

        routerA0:Router{}

        server0:StandardHost{}

        configurator: Ipv4NetworkConfigurator {}
    
    connections:
    
        client0.pppg++ <--> C2 <--> routerB0.pppg++;

        client1.pppg++ <--> C2 <--> routerB0.pppg++;

        client2.pppg++ <--> C2 <--> routerB0.pppg++;

        client3.pppg++ <--> C2 <--> routerB0.pppg++;

        client4.pppg++ <--> C2 <--> routerB0.pppg++;

        client5.pppg++ <--> C2 <--> routerB0.pppg++;

        client6.pppg++ <--> C2 <--> routerB0.pppg++;

        client7.pppg++ <--> C2 <--> routerB0.pppg++;

        client8.pppg++ <--> C2 <--> routerB0.pppg++;

        client9.pppg++ <--> C2 <--> routerB0.pppg++;

        client10.pppg++ <--> C2 <--> routerB0.pppg++;

        client11.pppg++ <--> C2 <--> routerB0.pppg++;

        client12.pppg++ <--> C2 <--> routerB0.pppg++;

        client13.pppg++ <--> C2 <--> routerB0.pppg++;

        client14.pppg++ <--> C2 <--> routerB0.pppg++;

        client15.pppg++ <--> C2 <--> routerB0.pppg++;

        client16.pppg++ <--> C2 <--> routerB0.pppg++;

        client17.pppg++ <--> C2 <--> routerB0.pppg++;

        client18.pppg++ <--> C2 <--> routerB0.pppg++;

        client19.pppg++ <--> C2 <--> routerB0.pppg++;

        client20.pppg++ <--> C2 <--> routerB0.pppg++;

        client21.pppg++ <--> C2 <--> routerB0.pppg++;

        client22.pppg++ <--> C2 <--> routerB0.pppg++;

        client23.pppg++ <--> C2 <--> routerB0.pppg++;

        client24.pppg++ <--> C2 <--> routerB0.pppg++;

        client25.pppg++ <--> C2 <--> routerB0.pppg++;

        client26.pppg++ <--> C2 <--> routerB0.pppg++;

        client27.pppg++ <--> C2 <--> routerB0.pppg++;

        client28.pppg++ <--> C2 <--> routerB0.pppg++;

        client29.pppg++ <--> C2 <--> routerB0.pppg++;

        client30.pppg++ <--> C2 <--> routerB0.pppg++;

        client31.pppg++ <--> C2 <--> routerB0.pppg++;

        client32.pppg++ <--> C2 <--> routerB0.pppg++;

        client33.pppg++ <--> C2 <--> routerB0.pppg++;

        client34.pppg++ <--> C2 <--> routerB0.pppg++;

        client35.pppg++ <--> C2 <--> routerB0.pppg++;

        client36.pppg++ <--> C2 <--> routerB0.pppg++;

        client37.pppg++ <--> C2 <--> routerB0.pppg++;

        client38.pppg++ <--> C2 <--> routerB0.pppg++;

        client39.pppg++ <--> C2 <--> routerB0.pppg++;

        client40.pppg++ <--> C2 <--> routerB0.pppg++;

        client41.pppg++ <--> C2 <--> routerB0.pppg++;

        client42.pppg++ <--> C2 <--> routerB0.pppg++;

        client43.pppg++ <--> C2 <--> routerB0.pppg++;

        client44.pppg++ <--> C2 <--> routerB0.pppg++;

        client45.pppg++ <--> C2 <--> routerB0.pppg++;

        client46.pppg++ <--> C2 <--> routerB0.pppg++;

        client47.pppg++ <--> C2 <--> routerB0.pppg++;

        client48.pppg++ <--> C2 <--> routerB0.pppg++;

        client49.pppg++ <--> C2 <--> routerB0.pppg++;

        client50.pppg++ <--> C2 <--> routerB0.pppg++;

        client51.pppg++ <--> C2 <--> routerB0.pppg++;

        client52.pppg++ <--> C2 <--> routerB0.pppg++;

        client53.pppg++ <--> C2 <--> routerB0.pppg++;

        client54.pppg++ <--> C2 <--> routerB0.pppg++;

        client55.pppg++ <--> C2 <--> routerB0.pppg++;

        client56.pppg++ <--> C2 <--> routerB0.pppg++;

        client57.pppg++ <--> C2 <--> routerB0.pppg++;

        client58.pppg++ <--> C2 <--> routerB0.pppg++;

        client59.pppg++ <--> C2 <--> routerB0.pppg++;

        client60.pppg++ <--> C2 <--> routerB0.pppg++;

        client61.pppg++ <--> C2 <--> routerB0.pppg++;

        client62.pppg++ <--> C2 <--> routerB0.pppg++;

        client63.pppg++ <--> C2 <--> routerB0.pppg++;

        client64.pppg++ <--> C2 <--> routerB0.pppg++;

        client65.pppg++ <--> C2 <--> routerB0.pppg++;

        client66.pppg++ <--> C2 <--> routerB0.pppg++;

        client67.pppg++ <--> C2 <--> routerB0.pppg++;

        client68.pppg++ <--> C2 <--> routerB0.pppg++;

        client69.pppg++ <--> C2 <--> routerB0.pppg++;

        client70.pppg++ <--> C2 <--> routerB0.pppg++;

        client71.pppg++ <--> C2 <--> routerB0.pppg++;

        client72.pppg++ <--> C2 <--> routerB0.pppg++;

        client73.pppg++ <--> C2 <--> routerB0.pppg++;

        client74.pppg++ <--> C2 <--> routerB0.pppg++;

        client75.pppg++ <--> C2 <--> routerB0.pppg++;

        client76.pppg++ <--> C2 <--> routerB0.pppg++;

        client77.pppg++ <--> C2 <--> routerB0.pppg++;

        client78.pppg++ <--> C2 <--> routerB0.pppg++;

        client79.pppg++ <--> C2 <--> routerB0.pppg++;

        client80.pppg++ <--> C2 <--> routerB0.pppg++;

        client81.pppg++ <--> C2 <--> routerB0.pppg++;

        client82.pppg++ <--> C2 <--> routerB0.pppg++;

        client83.pppg++ <--> C2 <--> routerB0.pppg++;

        client84.pppg++ <--> C2 <--> routerB0.pppg++;

        client85.pppg++ <--> C2 <--> routerB0.pppg++;

        client86.pppg++ <--> C2 <--> routerB0.pppg++;

        client87.pppg++ <--> C2 <--> routerB0.pppg++;

        client88.pppg++ <--> C2 <--> routerB0.pppg++;

        client89.pppg++ <--> C2 <--> routerB0.pppg++;

        client90.pppg++ <--> C2 <--> routerB0.pppg++;

        client91.pppg++ <--> C2 <--> routerB0.pppg++;

        client92.pppg++ <--> C2 <--> routerB0.pppg++;

        client93.pppg++ <--> C2 <--> routerB0.pppg++;

        client94.pppg++ <--> C2 <--> routerB0.pppg++;

        client95.pppg++ <--> C2 <--> routerB0.pppg++;

        client96.pppg++ <--> C2 <--> routerB0.pppg++;

        client97.pppg++ <--> C2 <--> routerB0.pppg++;

        client98.pppg++ <--> C2 <--> routerB0.pppg++;

        client99.pppg++ <--> C2 <--> routerB0.pppg++;

        client100.pppg++ <--> C2 <--> routerB1.pppg++;

        client101.pppg++ <--> C2 <--> routerB1.pppg++;

        client102.pppg++ <--> C2 <--> routerB1.pppg++;

        client103.pppg++ <--> C2 <--> routerB1.pppg++;

        client104.pppg++ <--> C2 <--> routerB1.pppg++;

        client105.pppg++ <--> C2 <--> routerB1.pppg++;

        client106.pppg++ <--> C2 <--> routerB1.pppg++;

        client107.pppg++ <--> C2 <--> routerB1.pppg++;

        client108.pppg++ <--> C2 <--> routerB1.pppg++;

        client109.pppg++ <--> C2 <--> routerB1.pppg++;

        client110.pppg++ <--> C2 <--> routerB1.pppg++;

        client111.pppg++ <--> C2 <--> routerB1.pppg++;

        client112.pppg++ <--> C2 <--> routerB1.pppg++;

        client113.pppg++ <--> C2 <--> routerB1.pppg++;

        client114.pppg++ <--> C2 <--> routerB1.pppg++;

        client115.pppg++ <--> C2 <--> routerB1.pppg++;

        client116.pppg++ <--> C2 <--> routerB1.pppg++;

        client117.pppg++ <--> C2 <--> routerB1.pppg++;

        client118.pppg++ <--> C2 <--> routerB1.pppg++;

        client119.pppg++ <--> C2 <--> routerB1.pppg++;

        client120.pppg++ <--> C2 <--> routerB1.pppg++;

        client121.pppg++ <--> C2 <--> routerB1.pppg++;

        client122.pppg++ <--> C2 <--> routerB1.pppg++;

        client123.pppg++ <--> C2 <--> routerB1.pppg++;

        client124.pppg++ <--> C2 <--> routerB1.pppg++;

        client125.pppg++ <--> C2 <--> routerB1.pppg++;

        client126.pppg++ <--> C2 <--> routerB1.pppg++;

        client127.pppg++ <--> C2 <--> routerB1.pppg++;

        client128.pppg++ <--> C2 <--> routerB1.pppg++;

        client129.pppg++ <--> C2 <--> routerB1.pppg++;

        client130.pppg++ <--> C2 <--> routerB1.pppg++;

        client131.pppg++ <--> C2 <--> routerB1.pppg++;

        client132.pppg++ <--> C2 <--> routerB1.pppg++;

        client133.pppg++ <--> C2 <--> routerB1.pppg++;

        client134.pppg++ <--> C2 <--> routerB1.pppg++;

        client135.pppg++ <--> C2 <--> routerB1.pppg++;

        client136.pppg++ <--> C2 <--> routerB1.pppg++;

        client137.pppg++ <--> C2 <--> routerB1.pppg++;

        client138.pppg++ <--> C2 <--> routerB1.pppg++;

        client139.pppg++ <--> C2 <--> routerB1.pppg++;

        client140.pppg++ <--> C2 <--> routerB1.pppg++;

        client141.pppg++ <--> C2 <--> routerB1.pppg++;

        client142.pppg++ <--> C2 <--> routerB1.pppg++;

        client143.pppg++ <--> C2 <--> routerB1.pppg++;

        client144.pppg++ <--> C2 <--> routerB1.pppg++;

        client145.pppg++ <--> C2 <--> routerB1.pppg++;

        client146.pppg++ <--> C2 <--> routerB1.pppg++;

        client147.pppg++ <--> C2 <--> routerB1.pppg++;

        client148.pppg++ <--> C2 <--> routerB1.pppg++;

        client149.pppg++ <--> C2 <--> routerB1.pppg++;

        client150.pppg++ <--> C2 <--> routerB1.pppg++;

        client151.pppg++ <--> C2 <--> routerB1.pppg++;

        client152.pppg++ <--> C2 <--> routerB1.pppg++;

        client153.pppg++ <--> C2 <--> routerB1.pppg++;

        client154.pppg++ <--> C2 <--> routerB1.pppg++;

        client155.pppg++ <--> C2 <--> routerB1.pppg++;

        client156.pppg++ <--> C2 <--> routerB1.pppg++;

        client157.pppg++ <--> C2 <--> routerB1.pppg++;

        client158.pppg++ <--> C2 <--> routerB1.pppg++;

        client159.pppg++ <--> C2 <--> routerB1.pppg++;

        client160.pppg++ <--> C2 <--> routerB1.pppg++;

        client161.pppg++ <--> C2 <--> routerB1.pppg++;

        client162.pppg++ <--> C2 <--> routerB1.pppg++;

        client163.pppg++ <--> C2 <--> routerB1.pppg++;

        client164.pppg++ <--> C2 <--> routerB1.pppg++;

        client165.pppg++ <--> C2 <--> routerB1.pppg++;

        client166.pppg++ <--> C2 <--> routerB1.pppg++;

        client167.pppg++ <--> C2 <--> routerB1.pppg++;

        client168.pppg++ <--> C2 <--> routerB1.pppg++;

        client169.pppg++ <--> C2 <--> routerB1.pppg++;

        client170.pppg++ <--> C2 <--> routerB1.pppg++;

        client171.pppg++ <--> C2 <--> routerB1.pppg++;

        client172.pppg++ <--> C2 <--> routerB1.pppg++;

        client173.pppg++ <--> C2 <--> routerB1.pppg++;

        client174.pppg++ <--> C2 <--> routerB1.pppg++;

        client175.pppg++ <--> C2 <--> routerB1.pppg++;

        client176.pppg++ <--> C2 <--> routerB1.pppg++;

        client177.pppg++ <--> C2 <--> routerB1.pppg++;

        client178.pppg++ <--> C2 <--> routerB1.pppg++;

        client179.pppg++ <--> C2 <--> routerB1.pppg++;

        client180.pppg++ <--> C2 <--> routerB1.pppg++;

        client181.pppg++ <--> C2 <--> routerB1.pppg++;

        client182.pppg++ <--> C2 <--> routerB1.pppg++;

        client183.pppg++ <--> C2 <--> routerB1.pppg++;

        client184.pppg++ <--> C2 <--> routerB1.pppg++;

        client185.pppg++ <--> C2 <--> routerB1.pppg++;

        client186.pppg++ <--> C2 <--> routerB1.pppg++;

        client187.pppg++ <--> C2 <--> routerB1.pppg++;

        client188.pppg++ <--> C2 <--> routerB1.pppg++;

        client189.pppg++ <--> C2 <--> routerB1.pppg++;

        client190.pppg++ <--> C2 <--> routerB1.pppg++;

        client191.pppg++ <--> C2 <--> routerB1.pppg++;

        client192.pppg++ <--> C2 <--> routerB1.pppg++;

        client193.pppg++ <--> C2 <--> routerB1.pppg++;

        client194.pppg++ <--> C2 <--> routerB1.pppg++;

        client195.pppg++ <--> C2 <--> routerB1.pppg++;

        client196.pppg++ <--> C2 <--> routerB1.pppg++;

        client197.pppg++ <--> C2 <--> routerB1.pppg++;

        client198.pppg++ <--> C2 <--> routerB1.pppg++;

        client199.pppg++ <--> C2 <--> routerB1.pppg++;

        routerB0.pppg++ <--> C1 <--> routerA0.pppg++;

        routerB1.pppg++ <--> C1 <--> routerA0.pppg++;

        routerA0.pppg++ <--> C0 <--> server0.pppg++;
}
