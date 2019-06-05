/**
 * @file DashClient.cc
 * @author Eduardo S Gama
 */

#include "DashClient.h"

#include <omnetpp/cexception.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cobjectfactory.h>
#include <omnetpp/cpar.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/cstringtokenizer.h>
#include <omnetpp/cwatch.h>
#include <omnetpp/platdep/platdefs.h>
#include <omnetpp/regmacros.h>
#include <omnetpp/simtime.h>
#include <omnetpp/simtime_t.h>

#include "../../../inet4/src/inet/applications/tcpapp/GenericAppMsg_m.h"
#include "../../../inet4/src/inet/common/InitStages.h"
#include "../../../inet4/src/inet/common/packet/chunk/Chunk.h"
#include "../../../inet4/src/inet/common/packet/chunk/FieldsChunk.h"
#include "../../../inet4/src/inet/common/Ptr.h"
#include "../../../inet4/src/inet/common/TimeTag_m.h"
#include "../../../inet4/src/inet/common/Units.h"
#include "../parser/pugixml.hpp"

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

    this->videoBuffer  = new VideoBuffer();
    this->dashplayback = new DashPlayback();
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

    // read Adaptive Video (AV) parameters
//    const char *str = par("video_packet_size_per_second").stringValue();
//    this->video_packet_size_per_second = cStringTokenizer(str).as IntVector(); //vector <1000,1500,2000,4000,8000,12000> in kbits

//    this->video_buffer_max_length = par("video_buffer_max_length"); // 10s
//    this->video_duration          = par("video_duration"); // 10m
//    this->manifest_size           = par("manifest_size"); // 100000


//    this->video_buffer_min_rebuffering = 3; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
//    this->video_buffer                 = 0;
//    this->video_playback_pointer       = 0;
//    this->video_current_quality_index  = 0;  // start with min quality
//    this->video_is_playing             = false;
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
    std::cout <<  "[handleTimer] Handle Timer " << msg->getKind() << std::endl;
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            connect();    // active OPEN

            break;

        case MSGKIND_SEND:
            sendRequest();
            // no scheduleAt(): next request will be sent when reply to this one
            // arrives (see socketDataArrived())
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void DashClient::decodePacket(Packet *vp)
{

}

void DashClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
    TcpAppBase::socketDataArrived(socket, msg, urgent);
    std::cout << "========================================" << std::endl;
    std::cout << "[socketDataArrived] Data Arrived Socket " << std::endl;
    std::cout << "Request Number=" << numRequestsToSend << std::endl;
    std::cout << "Total Length=" << msg->getTotalLength() << std::endl;
    std::cout << "Data Length=" << msg->getDataLength() << std::endl;
    std::cout << "Packets Received=" << packetsRcvd << std::endl;
    std::cout << "Bytes Received=" << videoBuffer->bytesRcvd << std::endl;
    std::cout << "Global Time=" << simTime() << std::endl;

    videoBuffer->bytesRcvd += bytesRcvd;

    if(bytesRcvd >= videoBuffer->segmentSize && numRequestsToSend > 0){
        std::cout << "Reply arrived" << std::endl;

        currentSegment->endSegment(simTime());

        this->videoBuffer->addSegment(*currentSegment);

        videoBuffer->bytesRcvd = bytesRcvd = 0;

        emit(this->DASH_seg_cmplt, this->videoBuffer->segIndex);
        rescheduleOrDeleteTimer(simTime(), MSGKIND_SEND);
    }
}

void DashClient::socketEstablished(TcpSocket *socket)
{
    TcpAppBase::socketEstablished(socket);

    if (!earlySend){
        prepareRequest();
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
    currentSegment = new Segment();

    MPDSegment &segment = (!earlySend) ? mpd->getLowRepresentation()
            : mpd->getHighRepresentation();

    currentSegment->setValues(segment.bandwidth/videoBuffer->numRequestsToSend, segment.frameRate, segment.width, segment.height);
    currentSegment->setSegmentNumber(this->videoBuffer->segIndex);

    videoBuffer->bytesRcvd = 0;
    videoBuffer->segmentSize = segment.bandwidth / videoBuffer->numRequestsToSend;
    videoBuffer->segmentframes = segment.frameRate;
    videoBuffer->reqtime = simTime();

    std::cout << "Bandwith=" << segment.bandwidth << std::endl;
    std::cout << "Segment Size=" << videoBuffer->segmentSize << std::endl;
}

void DashClient::sendRequest()
{
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

    sendPacket(packet);

    this->videoBuffer->segIndex++;
    numRequestsToSend--;
}

void DashClient::ReadMPD()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.mpd");
    
    if (!result){
    	std::cout << "ERROR" << std::endl;
        return;
    }
    
    dashplayback->title    = doc.child("MPD").child("ProgramInformation").child_value("Title");
    dashplayback->duration = doc.child("MPD").child("Period").child("AdaptationSet").child("SegmentTemplate").attribute("duration").as_int();
    
    
    std::cout << result.description()  << std::endl;
    std::cout << dashplayback->title    << std::endl;
	std::cout << dashplayback->duration << std::endl;
}
