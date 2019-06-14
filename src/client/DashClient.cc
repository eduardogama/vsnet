/**
 * @file DashClient.cc
 * @author Eduardo S Gama
 */

#include "DashClient.h"

#include <omnetpp/ccomponent.h>
#include <omnetpp/cexception.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cobjectfactory.h>
#include <omnetpp/cpacket.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/cwatch.h>
#include <omnetpp/regmacros.h>
#include <omnetpp/simtime.h>
#include <list>

#include "../../../inet4/src/inet/common/InitStages.h"
#include "../../../inet4/src/inet/common/packet/chunk/Chunk.h"
#include "../../../inet4/src/inet/common/packet/chunk/FieldsChunk.h"
#include "../../../inet4/src/inet/common/Ptr.h"
#include "../../../inet4/src/inet/common/TimeTag_m.h"
#include "../../../inet4/src/inet/common/Units.h"
#include "../server/DashAppMsg_m.h"

#define MSGKIND_CONNECT     0
#define MSGKIND_SEND        1
#define MSGKIND_VIDEO_PLAY  2


Define_Module(DashClient);

DashClient::DashClient()
{
}

DashClient::~DashClient()
{
}

void DashClient::initialize(int stage)
{
    TcpBasicClientApp::initialize(stage);

    if (stage != INITSTAGE_APPLICATION_LAYER)
        return;

    this->representation = new Representation();
    this->videoBuffer    = new VideoBuffer();
    this->dashplayback   = new DashPlayback();
    this->mpd            = new MPDRequestHandler();

    this->dashmanager = new DashManager();
    this->dashmanager->setRepresentation(representation);
    this->dashmanager->setMpd(mpd);

    this->mpd->ReadMPD("/home/futebol/github/vsnet/input/bbb_30fps");

    std::cout << "Video time="    << this->mpd->getMediaPresentationDuration()
              << " Segment time=" << this->mpd->getMinBufferTime() << std::endl;

    this->numRequestsToSend              = this->mpd->NumSegments();
    this->videoBuffer->numRequestsToSend = this->mpd->NumSegments();

    this->videoBuffer->segIndex     = 0;
    this->videoBuffer->playbbackPtr = 0;
    this->videoBuffer->minPlayBack  = 4;
    this->videoBuffer->maxBuffer    = 20;

    WATCH(this->videoBuffer);
    WATCH(this->videoBuffer->playbbackPtr);
    WATCH(this->videoBuffer->minPlayBack);

    WATCH(this->videoBuffer->segIndex);

    this->DASH_seg_cmplt = registerSignal("DASH_seg_cmplt");
    emit(this->DASH_seg_cmplt, this->videoBuffer->segIndex);

    this->DASH_video_is_playing = registerSignal("DASH_video_is_playing");
    emit(this->DASH_video_is_playing, this->videoBuffer->isReady());

    this->DASH_buffer_length = registerSignal("DASH_buffer_length");
    emit(this->DASH_buffer_length,this->videoBuffer->videostream->size());

    getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
}

void DashClient::rescheduleOrDeleteTimer(simtime_t d, short int msgKind) {

    cancelEvent(timeoutMsg);

    if(stopTime >= 0) {
        timeoutMsg->setKind(msgKind);
        scheduleAt(d, timeoutMsg);
    } else{
        delete timeoutMsg;
        timeoutMsg = nullptr;
    }
}

void DashClient::handleTimer(cMessage *msg)
{
    std::cout <<  "DashClient Handle Timer=" << msg->getKind() << std::endl;
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            connect();    // active OPEN

            break;
        case MSGKIND_SEND:
            prepareRequest();
            sendRequest();
            // no scheduleAt(): next request will be sent when reply to this one
            // arrives (see socketDataArrived())
            break;
        case MSGKIND_VIDEO_PLAY:
            cancelAndDelete(msg);

            this->videoBuffer->removeLastSegment();
            emit(DASH_buffer_length, this->videoBuffer->videostream->size());

            if(!this->videoBuffer->isEmpty()){
                emit(DASH_video_is_playing, this->videoBuffer->isReady());
            }

            break;
        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void DashClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
    printPacket(msg);
    videoBuffer->bytesRcvd += msg->getByteLength();

    queue.push(msg->peekData()); // get all data from the packet

    TcpAppBase::socketDataArrived(socket, msg, urgent);

    if((this->videoBuffer->bytesRcvd >= videoBuffer->segmentSize)
       && (numRequestsToSend > 0)) {

        const auto&  appmsg = queue.pop<DashAppMsg>(b(-1), Chunk::PF_ALLOW_NULLPTR);
        std::cout << "appmsg=" << appmsg->getRedirectAddress() << std::endl;

//        if(appmsg->getRedirectAddress() != nullptr || appmsg->getRedirectAddress() != "")
//            std::cout << "Change socket connection" << std::endl;

        this->c_segment->setEndTime(simTime());
        this->videoBuffer->addSegment(*c_segment);

        emit(this->DASH_seg_cmplt, this->videoBuffer->segIndex);
        rescheduleOrDeleteTimer(simTime(), MSGKIND_SEND);
    }

//    if(this->videoBuffer->isReady()) {
//        emit(DASH_video_is_playing, this->videoBuffer->isReady());
//
//        cMessage *videoPlaybackMsg = new cMessage("playback");
//        videoPlaybackMsg->setKind(MSGKIND_VIDEO_PLAY);
//
//        simtime_t d = this->videoBuffer->playingBackSeg().getDuration();
//
//        scheduleAt(simTime() + d, videoPlaybackMsg); // Time for remove segmente from the Buffer
//    }
}

void DashClient::socketEstablished(TcpSocket *socket)
{
    TcpAppBase::socketEstablished(socket);

    if (!earlySend){
        this->c_segment = this->dashmanager->LowRepresentation();

        this->videoBuffer->bytesRcvd     = 0;
        this->videoBuffer->segmentSize   = this->c_segment->getSegmentSize();
        this->videoBuffer->reqtime       = this->c_segment->getStartTime();

        std::cout << "Segment Size=" << this->videoBuffer->segmentSize << std::endl;

        sendRequest();

        earlySend = true;
    }
}

void DashClient::socketClosed(TcpSocket *socket)
{
    // TODO
    TcpBasicClientApp::socketClosed(socket);
}

void DashClient::socketFailure(TcpSocket *socket, int code) {
    // TODO
    TcpBasicClientApp::socketFailure(socket, code);
}

void DashClient::handleStartOperation(LifecycleOperation *operation)
{
    // TODO
    TcpBasicClientApp::handleStartOperation(operation);
}

void DashClient::handleStopOperation(LifecycleOperation *operation)
{
    // TODO
    TcpBasicClientApp::handleStopOperation(operation);
}

void DashClient::handleCrashOperation(LifecycleOperation *operation)
{
    // TODO
    TcpBasicClientApp::handleCrashOperation(operation);
}

void DashClient::prepareRequest()
{

    this->c_segment = this->dashmanager->BitRateAssigment();


    this->videoBuffer->bytesRcvd     = 0;
//    this->videoBuffer->segmentSize   = segment.mediaRange;
    this->videoBuffer->reqtime       = simTime();
    this->videoBuffer->segmentframes = this->representation->frameRate;

//    std::cout << "Id=" << segment.media << std::endl;
    std::cout << "Segment Size=" << this->videoBuffer->segmentSize << std::endl;
    sleep(4);
}

void DashClient::sendRequest()
{
    long requestLength = par("requestLength");
    long replyLength   = this->c_segment->getSegmentSize() + 52; // Fix it later | 52 is header size

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

    this->videoBuffer->segIndex++;
    numRequestsToSend--;
}

void DashClient::printPacket(Packet *msg)
{
    std::cout << "========================================" << std::endl;
    std::cout << "[socketDataArrived] Data Arrived Socket " << std::endl;
    std::cout << "Request Number=" << numRequestsToSend << std::endl;
    std::cout << "Total Length=" << msg->getTotalLength() << std::endl;
    std::cout << "Data Length=" << msg->getDataLength() << std::endl;
    std::cout << "Packets Received=" << packetsRcvd << std::endl;
    std::cout << "Bytes Received=" << videoBuffer->bytesRcvd << std::endl;
    std::cout << "Global Time=" << simTime() << std::endl;
}
