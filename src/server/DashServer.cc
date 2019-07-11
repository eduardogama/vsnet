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

#include "DashServer.h"

#include "inet/networklayer/common/L3AddressResolver.h"

#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/ModuleOperations.h"

#include "DashAppMsg_m.h"

#define MSGKIND_CONNECT    13
#define MSGKIND_SEND       14

Define_Module(DashServer);


inline ostream& operator<<(ostream& out, const VideoStreamDash& d)
{
    out << "client=" << d.clientAddr << ":" << d.clientPort
        << "  size=" << d.videoSize << "  pksent=" << d.numPkSent << "  bytesleft=" << d.bytesLeft;
    return out;
}

DashServer::DashServer() {
    // TODO Auto-generated constructor stub
}

DashServer::~DashServer() {
    // TODO Auto-generated destructor stub
}

void DashServer::initialize(int stage)
{
    TcpGenericServerApp::initialize(stage);

    if (stage != 3) return;

    this->mpd = MPDRequestHandler::getInstance();
}

void DashServer::handleMessage(cMessage *msg)
{
    cout << "DashServer handleMessage="
         << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind())
         << endl;

    if (msg->isSelfMessage()) {
        sendBack(msg);
    }
    else if (msg->getKind() == TCP_I_PEER_CLOSED) {
        // we'll close too, but only after there's surely no message
        // pending to be sent back in this connection
        int connId = check_and_cast<Indication *>(msg)->getTag<SocketInd>()->getSocketId();
        delete msg;
        auto request = new Request("close", TCP_C_CLOSE);
        request->addTagIfAbsent<SocketReq>()->setSocketId(connId);
        sendOrSchedule(request, delay + maxMsgDelay);
    }
    else if (msg->getKind() == TCP_I_DATA || msg->getKind() == TCP_I_URGENT_DATA) {
        Packet *packet = check_and_cast<Packet *>(msg);
        int connId = packet->getTag<SocketInd>()->getSocketId();
        ChunkQueue &queue = socketQueue[connId];
        auto chunk = packet->peekDataAt(B(0), packet->getTotalLength());
        queue.push(chunk);
        emit(packetReceivedSignal, packet);

        bool doClose = false;

//        std::cout << "========================================" << std::endl;
//        std::cout << "DashServer "
//                  << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind()) << std::endl
//                  << "Socket ID=" << connId << std::endl
//                  << "Total Length=" << packet->getTotalLength()
//                  << "Chunk Serialization="<< Chunk::enableImplicitChunkSerialization
//                  << std::endl;

        if(!queue.has<DashAppMsg>())
            return;

        while (const auto& appmsg = queue.pop<DashAppMsg>(b(-1), Chunk::PF_ALLOW_NULLPTR)) {
            msgsRcvd++;
            bytesRcvd += B(appmsg->getChunkLength()).get();
            B requestedBytes = appmsg->getExpectedReplyLength();

            simtime_t msgDelay = appmsg->getReplyDelay();

            if (msgDelay > maxMsgDelay)
                maxMsgDelay = msgDelay;

            if(this->streams.find(connId) == this->streams.end()){
                TcpSocket fog = ConnectFog(connId);
                initVideoStream(fog.getSocketId());

                this->streams[fog.getSocketId()].fogsocket = fog;
            }

            if (requestedBytes > B(0)) {
                Packet *outPacket = new Packet(msg->getName());
                outPacket->addTagIfAbsent<SocketReq>()->setSocketId(connId);
                outPacket->setKind(TCP_C_SEND);

                const auto& payload = makeShared<DashAppMsg>();
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                payload->setChunkLength(requestedBytes);
                payload->setExpectedReplyLength(B(0));
                payload->setReplyDelay(0);
                payload->setRedirectAddress("127.0.0.1");

                outPacket->insertAtBack(payload);

                sendOrSchedule(outPacket, delay + msgDelay);
            }
            if (appmsg->getServerClose()) {
                doClose = true;
                break;
            }
        }
        delete msg;

        if (doClose) {
            auto request = new Request("close", TCP_C_CLOSE);
            TcpCommand *cmd = new TcpCommand();
            request->addTagIfAbsent<SocketReq>()->setSocketId(connId);
            request->setControlInfo(cmd);
            sendOrSchedule(request, delay + maxMsgDelay);
        }
    }
    else if (msg->getKind() == TCP_I_AVAILABLE){
        socket.processMessage(msg);
    }
    else if (msg->getKind() == TCP_I_ESTABLISHED) {
        int connId = check_and_cast<Indication *>(msg)->getTag<SocketInd>()->getSocketId();

        cout << "Socket ID=" << connId << endl;

        if(this->streams.find(connId) != this->streams.end()) {
            handleConnection(connId);
        }
    }
    else {
        // some indication -- ignore
        EV_WARN << "drop msg: " << msg->getName() << ", kind:" << msg->getKind() << "(" << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind()) << ")\n";
        delete msg;
    }
}

TcpSocket DashServer::ConnectFog(int socketId)
{
    TcpSocket fogsocket;
    // we need a new connId if this is not the first connection
    fogsocket.renewSocket();

    const char *localAddress = "server";
    int localPort            = -1;
    fogsocket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    fogsocket.setCallback(this);
    fogsocket.setOutputGate(gate("socketOut"));

    // connect
    int connectPort = 1000;
    connectAddress = "cache";

    L3Address destination;
    L3AddressResolver().tryResolve(connectAddress.c_str(), destination);
    if (destination.isUnspecified()) {
        EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address\n";
    }
    else {
        EV_INFO << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

        fogsocket.connect(destination, connectPort);
    }

    cout << "Fog Socket ID=" << fogsocket.getSocketId() << endl;

    return fogsocket;
}

void DashServer::processStreamRequest(Packet *msg)
{
    // register video stream...
//    cMessage *timer = new cMessage("VideoStreamTmr");
//    VideoStreamDash *d = &streams[timer->getId()];
//    d->timer = timer;
//    d->videoSize = (*videoSize);
//    d->bytesLeft = d->videoSize;
//    d->numPkSent = 0;
//    ASSERT(d->videoSize > 0);
//    delete msg;
//
//    numStreams++;
//
//    // ... then transmit first packet right away
//    sendStreamData(timer);
}

void DashServer::sendStreamData(cMessage *timer)
{
//    auto it = streams.find(timer->getId());
//    if (it == streams.end())
//        throw cRuntimeError("Model error: Stream not found for timer");
//
//    VideoStreamDash *d = &(it->second);
//
//    // generate and send a packet
//    Packet *pkt = new Packet("VideoStrmPk");
//    long pktLen = *packetLen;
//
//    if (pktLen > d->bytesLeft)
//        pktLen = d->bytesLeft;
////    const auto& payload = makeShared<ByteCountChunk>(B(pktLen));
////    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
////    pkt->insertAtBack(payload);
//
////    emit(packetSentSignal, pkt);
////    socket.sendTo(pkt, d->clientAddr, d->clientPort);
////    socket.send(pkt);
//
//    d->bytesLeft -= pktLen;
//    d->numPkSent++;
//    numPkSent++;
//
//    // reschedule timer if there's bytes left to send
//    if (d->bytesLeft > 0) {
//        simtime_t interval = (*sendInterval);
//        scheduleAt(simTime() + interval, timer);
//    }
//    else {
//        streams.erase(it);
//        delete timer;
//    }
}

void DashServer::clearStreams()
{
//    for (auto & elem : streams)
//        cancelAndDelete(elem.second.timer);
//    streams.clear();
}

void DashServer::socketEstablished(TcpSocket* socket) {
}

void DashServer::socketDataArrived(TcpSocket* socket, Packet* msg, bool urgent)
{
    // *redefine* to perform or schedule next sending
    packetsRcvd++;
    bytesRcvd += msg->getByteLength();

    if (socket->getState() != TcpSocket::LOCALLY_CLOSED) {
        EV_INFO << "reply to last request arrived, closing session\n";
        socket->close();
    }

    cout << "entrou" << endl;

    emit(packetReceivedSignal, msg);
    delete msg;
}

void DashServer::socketClosed(TcpSocket* socket) {
}

void DashServer::socketFailure(TcpSocket* socket, int code) {
}

void DashServer::handleStartOperation(LifecycleOperation* operation) {
    simtime_t now = simTime();
    simtime_t start = max(startTime, now);
    if (timeoutMsg && ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))) {
        timeoutMsg->setKind(MSGKIND_CONNECT);
        scheduleAt(start, timeoutMsg);
    }
}

void DashServer::handleStopOperation(LifecycleOperation* operation) {
    cancelEvent(timeoutMsg);
    for (VideoStreamMap::iterator it=streams.begin(); it!=streams.end(); ++it){
        VideoStreamDash& vsm = it->second;

        vsm.fogsocket.close();
    }
}

void DashServer::socketPeerClosed(TcpSocket* socket) {

}

void DashServer::sendPacket(Packet *msg)
{
    int numBytes = msg->getByteLength();
    emit(packetSentSignal, msg);
    socket.send(msg);

    packetsSent++;
    bytesSent += numBytes;
}

void DashServer::initVideoStream(int socketId) {
    VideoStreamDash vsm;

    vsm.video_seg = vector<short int>(this->mpd->NumSegments(), 0);

    this->streams.insert(pair<long int, VideoStreamDash>(socketId,vsm));

    cout << "[Cloud] Segment Size=" << vsm.video_seg.size() << endl;
    for_each(vsm.video_seg.begin(), vsm.video_seg.end(), [](const short int& n){
        cout << n << " ";
    });
    cout << "Video Stream created ."<< endl;
}

void DashServer::handleConnection(int socketId) {
    cout << "Sending Segment to fog ... " << endl;

    VideoStreamDash& vsm = this->streams[socketId];

    vsm.video_seg[vsm.segIndex];

//    Segmentseg *seg = this->prepareRequest(vsm);
    Segment *seg = this->mpd->HighRepresentation(vsm.segIndex);
    sendRequest(seg);

    Packet* packet = preparePacket(vsm, seg);

    int numBytes = packet->getByteLength();


    socket.send(packet);

    packetsSent++;
    bytesSent += numBytes;

    vsm.segIndex++;
}

void DashServer::prepareRequest(VideoStreamDash& vsm) {
    Segment *seg = this->mpd->HighRepresentation(vsm.segIndex);
    cout << "Segment Size=" << seg->getSegmentSize() << endl;
}

Packet* DashServer::preparePacket(VideoStreamDash&vsm, Segment* seg) {
    long requestLength = par("requestLength");
    long replyLength   = seg->getSegmentSize();// + 52; // Fix it later | 52 is header size

    if (requestLength < 1)
        requestLength = 1;

    if (replyLength < 1)
        replyLength = 1;

    const auto& payload = makeShared<DashAppMsg>();
    Packet *packet = new Packet("data");
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    payload->setChunkLength(B(requestLength));
    payload->setExpectedReplyLength(B(0));
    payload->setServerClose(false);

    payload->setNum_segment(vsm.segIndex);
    payload->setBitrate(seg->getBitrate());
    payload->setResolution(seg->getQuality().c_str());

    cout << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
         << "remaining " << numRequestsToSend - 1 << " request" << endl;

    packet->insertAtBack(payload);

    return packet;
}

void DashServer::sendRequest(Segment *seg) {
    long requestLength = par("requestLength");
    long replyLength   = seg->getSegmentSize();// + 52; // Fix it later | 52 is header size

    if (requestLength < 1)
        requestLength = 1;

    if (replyLength < 1)
        replyLength = 1;

    const auto& payload = makeShared<DashAppMsg>();
    Packet *packet = new Packet("data");
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    payload->setChunkLength(B(requestLength));
    payload->setExpectedReplyLength(B(replyLength));
    payload->setServerClose(false);

    EV_INFO << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
            << "remaining " << numRequestsToSend - 1 << " request\n";

    std::cout << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
              << "remaining " << numRequestsToSend - 1 << " request\n";

    packet->insertAtBack(payload);
    sendPacket(packet);
}

void DashServer::handleCrashOperation(LifecycleOperation* operation) {
    cout << "entrou []" << endl;
    cancelEvent(timeoutMsg);
    if (operation->getRootModule() != getContainingNode(this))
        socket.destroy();
}
//void DashServer::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
//{
////    TcpAppBase::socketDataArrived(socket, msg, urgent);
//
//    // Should be a HttpReplyMessage
//    EV_DEBUG << "Socket data arrived on connection " << socket->getSocketId() << ". Message=" << msg->getName() << ", kind=" << msg->getKind() << endl;
//
//    if(streams.find(socket->getSocketId()) == streams.end())
//    {
//        VideoStreamDash stream;
//
//        stream.clientAddr = socket->getRemoteAddress();
//        stream.clientPort = socket->getRemotePort();
//
//        std::pair<long int, VideoStreamDash> ps(socket->getSocketId(), stream);
//        this->streams.insert(ps);
//    }
//}
