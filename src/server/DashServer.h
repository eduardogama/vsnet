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

#include <map>
#include <algorithm>

//#include "services/CacheService.h"
#include "inet/applications/tcpapp/TcpGenericServerApp.h"
#include "inet/common/lifecycle/ILifecycle.h"

#include "client/MPDRequestHandler.h"
#include "mgmt/DashManager.h"

using namespace inet;


struct VideoStreamDash
{
    TcpSocket fogsocket;
    cMessage *timer = nullptr;    // self timer msg
    L3Address clientAddr;    // client address
    int clientPort = -1;    // client TCP port
    long videoSize = 0;    // total size of video

    long bytesLeft = 0;    // bytes left to transmit
    long numPkSent = 0;    // number of packets sent
    int segIndexFog = 20;
    vector<short int> video_seg;
};

typedef std::map<long int, VideoStreamDash> VideoStreamMap;

//typedef std::map<long int, CacheService> FogMap;

class DashServer : public TcpGenericServerApp, public TcpSocket::ICallback {
    public:
        DashServer();
        virtual ~DashServer();

    protected:
        virtual void initialize(int stage) override;

        virtual void handleMessage(cMessage *msg) override;

        virtual void clearStreams();

        virtual void processStreamRequest(Packet *msg);
        virtual void sendStreamData(cMessage *timer);

        TcpSocket ConnectFog(int socketId);

        void initVideoStream(int socketId);
        void handleFogConnection(int socketId);

        void prepareRequest(VideoStreamDash& vsm);
        Packet* preparePacket(VideoStreamDash&vsm, Segment* seg);
        void sendRequest(Segment* seg);
        void sendPacket(Packet* msg);

        virtual void socketEstablished(TcpSocket *socket) override;
        virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;
        virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override { socket->accept(availableInfo->getNewSocketId()); }
        virtual void socketClosed(TcpSocket *socket) override;
        virtual void socketFailure(TcpSocket *socket, int code) override;
        virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override { }
        virtual void socketDeleted(TcpSocket *socket) override {}
        virtual void socketPeerClosed(TcpSocket *socket) override;

        virtual void handleStartOperation(LifecycleOperation *operation);
        virtual void handleStopOperation(LifecycleOperation *operation);
        virtual void handleCrashOperation(LifecycleOperation *operation);

    protected:

        MPDRequestHandler *mpd;

        string connectAddress;
        int localPort = -1;

        // State
        VideoStreamMap streams;

        // Network State
//        FogMap fognodes;

        map<int, NodeType> nodeMap;

        // Parameters
        cPar *sendInterval = nullptr;
        cPar *packetLen = nullptr;
        cPar *videoSize = nullptr;

        // Statistics
        unsigned int numStreams = 0;             // Number of video streams served
        unsigned long numPkSent = 0;             // Total number of packets sent
        static simsignal_t reqStreamBytesSignal; // Length of video streams served

        // statistics
        int numSessions;
        int numBroken;
        int packetsSent;
        int packetsRcvd;
        int bytesSent;
        int bytesRcvd;

        // AppClient
        cMessage *timeoutMsg = nullptr;
        bool earlySend = false;    // if true, don't wait with sendRequest() until established()
        int numRequestsToSend = 0;    // requests to send in this session

        simtime_t startTime;
        simtime_t stopTime;
};

#endif /* DASH_DASHSERVER_H_ */
