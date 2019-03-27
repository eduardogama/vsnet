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

package vsnet.dash;

import inet.applications.contract.IApp;


simple DashServer like IApp
{
    parameters:
        string localAddress = default("");  // may be left empty ("")
        int    localPort    = default(-1); 	// port number to listen on
        
        double replyDelay @unit(s) = default(0s);
        
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
        
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
        
        // General statistics
		@signal[DASHBufferLength](type="long");
        @statistic[DASHBufferLength](title="Video buffer length (in seconds)"; record=vector);
        
        @signal[DASHQualityLevel](type="int");
        @statistic[DASHQualityLevel](title="Requested quality level"; record=vector);
        
        @signal[DASHVideoPlaybackPointer](type="int");
        @statistic[DASHVideoPlaybackPointer](title="Video playback pointer"; record=vector);
        
        @signal[DASHVideoPlaybackStatus](type="bool");
        @statistic[DASHVideoPlaybackStatus](title="Video playback status (playing=1, buffering=0)"; record=vector);
        
    gates:
        input socketIn @labels(TcpCommand/up);
        output socketOut @labels(TcpCommand/down);
}
