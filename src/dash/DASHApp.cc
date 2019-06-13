/*
 * DASHApp.cpp
 *
 *  Created on: 20 de mar de 2019
 *      Author: futebol
 */

#include "DASHApp.h"

#include "inet/applications/tcpapp/GenericAppMsg_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TimeTag_m.h"


#define MSGKIND_CONNECT     0
#define MSGKIND_SEND        1
#define MSGKIND_VIDEO_PLAY  2


Define_Module(DASHApp);

DASHApp::DASHApp() {
    timeoutMsg = NULL;
    // TODO Auto-generated constructor stub
}

DASHApp::~DASHApp() {
    cancelAndDelete(timeoutMsg);
    // TODO Auto-generated destructor stub
}

void DASHApp::initialize(int stage) {
    HttpNodeBase::initialize(stage);

    if (stage != 3) return;

    this->videoBuffer  = new VideoBuffer();
    this->mpd          = new MPDRequestHandler();

    this->mpd->ReadMPD("/home/eduardo/github/vsnet/input/sample.mpd");

    std::cout << "Video time=" << this->mpd->getMediaPresentationDuration() << " Segment=" << this->mpd->getMaxSegmentDuration() << std::endl;

    this->numRequestsToSend = this->mpd->getMediaPresentationDuration() / mpd->getMaxSegmentDuration();

    this->videoBuffer->numRequestsToSend = numRequestsToSend;
    this->videoBuffer->segIndex = 0;

    this->videoBuffer->playbbackPtr = 0;
    this->videoBuffer->minPlayBack  = 4;
    this->videoBuffer->maxBuffer    = 20;
    this->videoBuffer->isPlaying    = false;

    WATCH(this->videoBuffer);
    WATCH(this->videoBuffer->playbbackPtr);
    WATCH(this->videoBuffer->minPlayBack);

    WATCH(this->videoBuffer->segIndex);

    this->DASH_seg_cmplt = registerSignal("DASH_seg_cmplt");
    emit(this->DASH_seg_cmplt, this->videoBuffer->segIndex);

    getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
}

void DASHApp::prepareRequest()
{
    currentSegment = new Segment();

    MPDSegment &segment = (!earlySend) ? mpd->getLowRepresentation()
            : mpd->getHighRepresentation();

    currentSegment->setValues(segment.mediaRange, representation->frameRate, representation->width, representation->height);
    currentSegment->setSegmentNumber(this->videoBuffer->segIndex);

    videoBuffer->bytesRcvd = 0;
    videoBuffer->segmentSize = segment.mediaRange;
    videoBuffer->segmentframes = representation->frameRate;
    videoBuffer->reqtime = simTime();

    std::cout << "Bandwith=" << segment.mediaRange << std::endl;
    std::cout << "Segment Size=" << videoBuffer->segmentSize << std::endl;
}

void DASHApp::sendRequest() {

    long requestLength = par("requestLength");
    long replyLength   = videoBuffer->segmentSize;// + 52; // Fix it later | 52 is header size

    if (requestLength < 1)
        requestLength = 1;

    if (replyLength < 1)
        replyLength = 1;

    const auto& payload = makeShared<GenericAppMsg>();
    Packet *packet = new Packet("data");
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    payload->setChunkLength(B(requestLength));
    payload->setExpectedReplyLength(B(replyLength));
    payload->setServerClose(false);
    packet->insertAtBack(payload);

    EV_INFO << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
            << "remaining " << numRequestsToSend - 1 << " request\n";

    std::cout << "sending request with " << requestLength << " bytes, expected reply length " << replyLength << " bytes,"
              << "remaining " << numRequestsToSend - 1 << " request\n";

    this->videoBuffer->segIndex++;
    numRequestsToSend--;
}

void DASHApp::handleMessage(cMessage *msg) {
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            EV << "STARTING SESSION" << endl;

            break;
        case MSGKIND_SEND:
            sendRequest();

            break;
        case MSGKIND_VIDEO_PLAY:
            EV<< "=> Video play event";
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void DASHApp::socketEstablished(TcpSocket *socket) {
    // perform first request
    sendRequest();
}

void DASHApp::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) {
}

void DASHApp::socketClosed(TcpSocket *socket) {
    // Nothing to do here...
}

void DASHApp::socketPeerClosed(TcpSocket *socket) {
    // Nothing to do here...
}

void DASHApp::socketFailure(TcpSocket *socket, int code) {
    // TODO
}

void DASHApp::socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo)
{

}

void DASHApp::socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status)
{

}

void DASHApp::socketDeleted(TcpSocket *socket)
{

}

