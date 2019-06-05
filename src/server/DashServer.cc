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


Define_Module(DashServer);

DashServer::DashServer() {
    // TODO Auto-generated constructor stub
}

DashServer::~DashServer() {
    // TODO Auto-generated destructor stub
}

void DashServer::initialize(int stage)
{
//    TcpServerHostApp::initialize(stage);
//
//    if (stage != 3) return;
//
//    if (stage == INITSTAGE_LOCAL) {
//        sendInterval = &par("sendInterval");
//        packetLen = &par("packetLen");
//        videoSize = &par("videoSize");
//        localPort = par("localPort");
//
//        // statistics
//        numStreams = 0;
//        numPkSent = 0;
//
//        WATCH_MAP(streams);
//    }
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
