/*
 * CacheService.cpp
 *
 *  Created on: 2 de jun de 2019
 *      Author: eduardo
 */

#include "CacheService.h"

#include <omnetpp/ccomponent.h>
#include <omnetpp/cenum.h>
#include <omnetpp/cexception.h>
#include <omnetpp/checkandcast.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cnamedobject.h>
#include <omnetpp/cpar.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/cwatch.h>
#include <omnetpp/simtime.h>
#include <sstream>
#include <string>

#include "inet/applications/common/SocketTag_m.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"
#include "inet/common/InitStages.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/chunk/Chunk.h"
#include "inet/common/packet/chunk/FieldsChunk.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/Ptr.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/Units.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpCommand_m.h"

#include "server/DashAppMsg_m.h"


Define_Module(CacheService);


CacheService::CacheService() {
    // TODO Auto-generated constructor stub
}

CacheService::~CacheService() {
    // TODO Auto-generated destructor stub
}

void CacheService::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        delay = par("replyDelay");
        maxMsgDelay = 0;

        //statistics
        msgsRcvd = msgsSent = bytesRcvd = bytesSent = 0;

        WATCH(msgsRcvd);
        WATCH(msgsSent);
        WATCH(bytesRcvd);
        WATCH(bytesSent);
    } else if (stage == INITSTAGE_APPLICATION_LAYER) {

        const char *localAddress = par("localAddress");
        int localPort            = par("localPort");

        socket.setOutputGate(gate("socketOut"));
        socket.bind(localAddress[0] ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);
        socket.listen();

        cModule *node = findContainingNode(this);
        NodeStatus *nodeStatus = node ? check_and_cast_nullable<NodeStatus *>(node->getSubmodule("status")) : nullptr;
        bool isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;

        if (!isOperational)
            throw cRuntimeError("This module doesn't support starting in node DOWN state");
    }
}

void CacheService::handleMessage(cMessage *msg)
{
    if (msg->getKind() == TCP_I_PEER_CLOSED) {
        // we'll close too, but only after there's surely no message
        // pending to be sent back in this connection
        int connId = check_and_cast<Indication *>(msg)->getTag<SocketInd>()->getSocketId();
        delete msg;
        auto request = new Request("close", TCP_C_CLOSE);
        request->addTagIfAbsent<SocketReq>()->setSocketId(connId);
//        sendOrSchedule(request, delay + maxMsgDelay);
    }
    else if (msg->getKind() == TCP_I_DATA || msg->getKind() == TCP_I_URGENT_DATA) {
        Packet *packet = check_and_cast<Packet *>(msg);
        int connId = packet->getTag<SocketInd>()->getSocketId();
        ChunkQueue &queue = socketQueue[connId];
//        auto chunk = packet->peekDataAt(B(0), packet->getTotalLength());

        queue.push(packet->peekData());

//        emit(packetReceivedSignal, packet);

        if(!queue.has<DashAppMsg>()) {
            delete msg;
            return;
        }

        bool doClose = false;
        while (const auto& appmsg = queue.pop<DashAppMsg>(b(-1), Chunk::PF_ALLOW_NULLPTR)) {
            if(!strcmp(appmsg->getSender(), "server")) {
                cout << simTime()
                     << " [Fog Node] Segment Length=" << B(appmsg->getChunkLength()).get()
                     << " Expected Length=" << packet->getTotalLength()
                     << " Expected Reply Length=" << appmsg->getExpectedReplyLength()
                     << " Segment Number=" << appmsg->getNum_segment()
                     << endl;

                Segment *seg = new Segment();

                cout << "entrou\n";

                seg->setQuality(appmsg->getResolution());
                seg->setBitrate(appmsg->getBitrate());
                seg->setSegmentNumber(appmsg->getNum_segment());

                cout << "entrou\n";

                storeSegmentVideo(appmsg->getMedia(), seg);

                cout << "entrou\n";
            }

            msgsRcvd++;
            bytesRcvd += B(appmsg->getChunkLength()).get();


            cout << "entrouiojq jio\n";

            B requestedBytes = appmsg->getExpectedReplyLength();
            simtime_t msgDelay = appmsg->getReplyDelay();

            if (msgDelay > maxMsgDelay)
                maxMsgDelay = msgDelay;

            cout << "requestedBytes=" << requestedBytes << endl;

            if (requestedBytes > B(0)) {

                Packet *outPacket = new Packet(msg->getName());
                outPacket->addTagIfAbsent<SocketReq>()->setSocketId(connId);
                outPacket->setKind(TCP_C_SEND);
                const auto& payload = makeShared<DashAppMsg>();
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                payload->setChunkLength(requestedBytes);
                payload->setExpectedReplyLength(B(0));
                payload->setReplyDelay(0);

                payload->setSender("fog");
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

Packet *CacheService::PrepareRequest(TcpSocket *socket, Packet *msg)
{
    std::stringstream req;

    if(true) //if(!msg->HasByteRange())
    {
        req << "GET " << "(segment path)" << " HTTP/1.1\r\n" <<
            "Host: " << socket->getSocketId() << "\r\n\r\n";
    }
    else
    {
        req << "GET " << "segment path" << " HTTP/1.1\r\n" <<
            "Host: " <<  socket->getSocketId() << "\r\n";
//          "Range: bytes=" << msg->StartByte() << "-" << chunk->EndByte() << "\r\n\r\n";
    }

    std::string request = req.str();

    return new Packet();
}

void CacheServiceBase::insertSegment(std::string path_seg)
{
    // TODO
}

void CacheService::storeSegmentVideo(const char* movie, Segment *seg)
{
    if(this->streams.find(movie) == this->streams.end()) {
        this->streams.insert(make_pair(movie, VideoSegments()));
    }

    this->streams[movie][seg->getSegmentNumber()] = *seg;
}
