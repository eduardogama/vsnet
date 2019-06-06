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

#include <omnetpp/cenum.h>
#include <omnetpp/checkandcast.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cnamedobject.h>
#include <omnetpp/cobjectfactory.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/cstlwatch.h>
#include <omnetpp/regmacros.h>
#include <omnetpp/simtime.h>
#include <omnetpp/simtime_t.h>
#include <iostream>

#include "../../../inet4/src/inet/applications/common/SocketTag_m.h"
#include "../../../inet4/src/inet/common/InitStages.h"
#include "../../../inet4/src/inet/common/packet/chunk/Chunk.h"
#include "../../../inet4/src/inet/common/packet/chunk/FieldsChunk.h"
#include "../../../inet4/src/inet/common/packet/ChunkQueue.h"
#include "../../../inet4/src/inet/common/packet/Message.h"
#include "../../../inet4/src/inet/common/packet/Packet.h"
#include "../../../inet4/src/inet/common/Ptr.h"
#include "../../../inet4/src/inet/common/Simsignals.h"
#include "../../../inet4/src/inet/common/Units.h"
#include "../../../inet4/src/inet/transportlayer/contract/tcp/TcpCommand_m.h"
#include "DashAppMsg_m.h"

Define_Module(DashServer);


inline std::ostream& operator<<(std::ostream& out, const VideoStreamDash& d)
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

    if (stage == INITSTAGE_LOCAL) {
        WATCH_MAP(streams);
    }
}

void DashServer::handleMessage(cMessage *msg)
{
    std::cout << "DashServer handleMessage="
              << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind())
              << std::endl;

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

        while (const auto& appmsg = queue.pop<DashAppMsg>(b(-1), Chunk::PF_ALLOW_NULLPTR)) {
            msgsRcvd++;
            bytesRcvd += B(appmsg->getChunkLength()).get();
            B requestedBytes = appmsg->getExpectedReplyLength();

            simtime_t msgDelay = appmsg->getReplyDelay();
            if (msgDelay > maxMsgDelay)
                maxMsgDelay = msgDelay;

            if (requestedBytes > B(0)) {
                Packet *outPacket = new Packet(msg->getName());
                outPacket->addTagIfAbsent<SocketReq>()->setSocketId(connId);
                outPacket->setKind(TCP_C_SEND);

                const auto& payload = makeShared<DashAppMsg>();
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                payload->setChunkLength(requestedBytes);
                payload->setExpectedReplyLength(B(0));
                payload->setReplyDelay(0);

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
    else if (msg->getKind() == TCP_I_AVAILABLE)
        socket.processMessage(msg);
    else {
        // some indication -- ignore
        EV_WARN << "drop msg: " << msg->getName() << ", kind:" << msg->getKind() << "(" << cEnum::get("inet::TcpStatusInd")->getStringFor(msg->getKind()) << ")\n";
        delete msg;
    }
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
