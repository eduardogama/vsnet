/**
 * @file DashClient.cc
 * @author Eduardo S Gama
 */

#include "DashClient.h"

#include "inet/applications/tcpapp/GenericAppMsg_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TimeTag_m.h"


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

    EV_INFO << getFullPath() << "Initializing DashClient ... \n";

	mpd.ReadMPD("/home/futebol/github/vsnet/bin/sample.mpd");

	// read Adaptive Video (AV) parameters
    const char *str = par("video_packet_size_per_second").stringValue();
    video_packet_size_per_second = cStringTokenizer(str).asIntVector(); //vector <1000,1500,2000,4000,8000,12000> in kbits

    video_buffer_max_length = par("video_buffer_max_length"); // 10s
    video_duration          = par("video_duration"); // 30s
    manifest_size           = par("manifest_size"); // 100000

    numRequestsToSend = mpd.getSegments().size();

    video_buffer_min_rebuffering = 3; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
    video_buffer                 = 0;
    video_playback_pointer       = 0;
    video_current_quality_index  = 0;  // start with min quality
    video_is_playing             = false;

	WATCH(video_buffer);
    WATCH(video_playback_pointer);

//    if (stopTime >= SIMTIME_ZERO && stopTime < startTime){
//        throw cRuntimeError("Invalid startTime/stopTime parameters");
//    }
//    timeoutMsg = new cMessage("timer");

    getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
}

void DashClient::decodePacket(Packet *vp)
{

}

void DashClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
    std::cout << "========================================" << std::endl;
    std::cout << "[socketDataArrived] Data Arrived Socket " << std::endl;
    std::cout << "Total Length=" << msg->getTotalLength() << std::endl;
    std::cout << "Data Length=" << msg->getDataLength() << std::endl;

    TcpAppBase::socketDataArrived(socket, msg, urgent);

    std::cout << "Packets Received=" << packetsRcvd << std::endl;
    std::cout << "Bytes Received=" << bytesRcvd << std::endl;

    if(numRequestsToSend > 0){
        std::cout << "Reply arrived" << std::endl;
        if(timeoutMsg){
            //Send new request
            simtime_t d = simTime();
            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        }
    }
}



void DashClient::socketEstablished(TcpSocket *socket)
{
    std::cout << "[socketEstablished] Socket being established ... " << std::endl;

    TcpAppBase::socketEstablished(socket);

    if (!earlySend)
        sendRequest();

    numRequestsToSend--;
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

void DashClient::sendRequest()
{
    long requestLength = par("requestLength");
    long replyLength   = par("replyLength"); // Fix it later

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

            // significance of earlySend: if true, data will be sent already
            // in the ACK of SYN, otherwise only in a separate packet (but still
            // immediately)
            if (earlySend)
                sendRequest();

            std::cout << "[handleTimer] Connection established ." << std::endl;

            break;

        case MSGKIND_SEND:
            sendRequest();
            numRequestsToSend--;
            // no scheduleAt(): next request will be sent when reply to this one
            // arrives (see socketDataArrived())
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void DashClient::ReadMPD()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.mpd");
    
    if (!result){
    	std::cout << "ERROR" << std::endl;
        return;
    }
    
    dashplayback.title    = doc.child("MPD").child("ProgramInformation").child_value("Title");
    dashplayback.duration = doc.child("MPD").child("Period").child("AdaptationSet").child("SegmentTemplate").attribute("duration").as_int();
    
    
    std::cout << result.description()  << std::endl;
    std::cout << dashplayback.title    << std::endl;
	std::cout << dashplayback.duration << std::endl;	
}
