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

package vsnet.services;

import inet.applications.contract.IApp;

simple CacheService like IApp
{
    parameters:
        string localAddress   = default(""); // may be left empty ("")
        int    localPort 	  = default(-1); // port number to listen on
        string connectAddress = default("");  // server address (may be symbolic)
        int    connectPort    = default(1000); // port number to connect to
        
        double replyDelay @unit(s) = default(0s);
        
        double startTime @unit(s) = default(1s); // time first session begins
        double stopTime  @unit(s) = default(-1s);  // time of finishing sending, negative values mean forever
        
        volatile int numRequestsPerSession  = default(1);  // number of requests sent per session
        volatile int requestLength @unit(B) = default(200B); // length of a request
        volatile int replyLength @unit(B)   = default(1MiB); // length of a reply

//        volatile double thinkTime @unit(s); // time gap between requests
//        volatile double idleInterval @unit(s); // time gap between sessions

        volatile double reconnectInterval @unit(s) = default(30s);  // if connection breaks, waits this much before trying to reconnect
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
        @signal[connect](type=long);  // 1 for open, -1 for close
    gates:
        input socketIn @labels(TcpCommand/up);
        output socketOut @labels(TcpCommand/down);    
}