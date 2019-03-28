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

#ifndef DASH_DASHSERVER_H_
#define DASH_DASHSERVER_H_

//#include "inet/common/INETDefs.h"


#include "inet/applications/tcpapp/TcpGenericServerApp.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"


using namespace inet;


class DashServer :  public TcpGenericServerApp {
public:
    DashServer();
    virtual ~DashServer();

    struct VideoStreamDash
    {
        cMessage *timer = nullptr;    // self timer msg
        L3Address clientAddr;    // client address
        int clientPort = -1;    // client UDP port
        long videoSize = 0;    // total size of video
        long bytesLeft = 0;    // bytes left to transmit
        long numPkSent = 0;    // number of packets sent
    };

protected:
    virtual void initialize(int stage) override;

    virtual void clearStreams();

    virtual void processStreamRequest(Packet *msg);
    virtual void sendStreamData(cMessage *timer);


//    virtual void handleStartOperation(LifecycleOperation *operation) override;
//    virtual void handleStopOperation(LifecycleOperation *operation) override;
//    virtual void handleCrashOperation(LifecycleOperation *operation) override;


protected:
    typedef std::map<long int, VideoStreamDash> VideoStreamMap;

    int localPort = -1;

    // State
    VideoStreamMap streams;

    // Parameters
    cPar *sendInterval = nullptr;
    cPar *packetLen = nullptr;
    cPar *videoSize = nullptr;

    // Statistics
    unsigned int numStreams = 0;             // Number of video streams served
    unsigned long numPkSent = 0;             // Total number of packets sent
    static simsignal_t reqStreamBytesSignal; // Length of video streams served

};

#endif /* DASH_DASHSERVER_H_ */
