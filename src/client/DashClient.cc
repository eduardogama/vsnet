/**
 * @file DashClient.cc
 * @author Eduardo S Gama
 */

#include "DashClient.h"

#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "server/DashAppMsg_m.h"
#include "inet/applications/tcpapp/GenericAppMsg_m.h"


#define MSGKIND_CONNECT     0
#define MSGKIND_SEND        1
#define MSGKIND_VIDEO_PLAY  2
#define MSGKIND_CONNECT_FOG 3

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
    this->mpd            = MPDRequestHandler::getInstance();

    this->dashmanager = new DashManager();
    this->dashmanager->setMpd(this->mpd);

    this->mpd->ReadMPD("/home/eduardogama/Dropbox/bbb_30fps");

    this->dashmanager->setQualities(&(this->mpd->getQuality()));
    this->dashmanager->setRepresentation(&(this->mpd->getRepresentation()));

    cout << "Video time="    << this->mpd->getMediaPresentationDuration()
              << " Segment time=" << this->mpd->getMinBufferTime() << endl;

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

    this->connectAddress = par("connectAddress").stringValue();

    getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
}

void DashClient::rescheduleOrDeleteTimer(simtime_t d, short int msgKind) {

    cancelEvent(timeoutMsg);

    if(stopTime >= 0) {
        timeoutMsg->setKind(msgKind);
        scheduleAt(d, timeoutMsg);
    } else {
        delete timeoutMsg;
        timeoutMsg = nullptr;
    }
}

void DashClient::handleTimer(cMessage *msg)
{
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
        case MSGKIND_CONNECT_FOG:
            close();

            Connect();
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

void DashClient::Connect()
{
    // we need a new connId if this is not the first connection
    socket.renewSocket();

    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    // connect
    const char *connectAddress = this->connectAddress.c_str();
    int connectPort = par("connectPort");

    L3Address destination;
    L3AddressResolver().tryResolve(connectAddress, destination);
    if (destination.isUnspecified()) {
        EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address\n";
    }
    else {
        EV_INFO << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

        socket.connect(destination, connectPort);

        numSessions++;
        emit(connectSignal, 1L);
    }
}

void DashClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
//    printPacket(msg);
    videoBuffer->bytesRcvd += msg->getByteLength();

    queue.push(msg->peekData()); // get all data from the packet

    TcpAppBase::socketDataArrived(socket, msg, urgent);

    if((this->videoBuffer->bytesRcvd >= videoBuffer->segmentSize)
       && (numRequestsToSend > 0)) {

        const auto&  appmsg = queue.pop<DashAppMsg>(b(-1), Chunk::PF_ALLOW_SERIALIZATION);
//        std::cout << "appmsg=" << appmsg->getRedirectAddress() << std::endl;

//        if(appmsg->getRedirectAddress() != nullptr || appmsg->getRedirectAddress() != "")
//            std::cout << "Change socket connection" << std::endl;

//        cout << simTime()
//             << " [Client Node] Segment Length=" << this->videoBuffer->bytesRcvd
//             << endl;

        this->c_segment->setEndTime(simTime());
        this->videoBuffer->addSegment(*c_segment);

        emit(this->DASH_seg_cmplt, this->videoBuffer->segIndex);

        this->timeoutMsg->setKind(MSGKIND_SEND);
        scheduleAt(simTime(), this->timeoutMsg);
    }

//    TcpAppBase::socketDataArrived(socket, msg, urgent);
//
//    if (numRequestsToSend > 0) {
//        EV_INFO << "reply arrived\n";
//
//        if (timeoutMsg) {
//            simtime_t d = simTime() + par("thinkTime");
//            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
//        }
//    }
//    else if (socket->getState() != TcpSocket::LOCALLY_CLOSED) {
//        EV_INFO << "reply to last request arrived, closing session\n";
//        close();
//    }

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

        this->videoBuffer->res           = "360p";
        this->videoBuffer->bytesRcvd     = 0;
        this->videoBuffer->segmentSize   = this->c_segment->getSegmentSize();
        this->videoBuffer->reqtime       = this->c_segment->getStartTime();

//        cout << "Segment Size=" << this->videoBuffer->segmentSize << endl;

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
    this->c_segment = this->dashmanager->BitRateAssigment(this->videoBuffer);

    this->videoBuffer->bytesRcvd     = 0;
    this->videoBuffer->segmentSize   = this->c_segment->getSegmentSize();
    this->videoBuffer->reqtime       = this->c_segment->getStartTime();
    this->videoBuffer->res           = this->c_segment->getQuality();

//    cout << "Segment Size=" << this->videoBuffer->segmentSize << endl;
}

void DashClient::sendRequest()
{
    long requestLength = par("requestLength");
    long replyLength   = this->c_segment->getSegmentSize();// + 52; // Fix it later | 52 is header size

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

    cout << simTime() << " [Client Node] Sending request with " << requestLength << " bytes and  " << this->videoBuffer->res
            << " resolution, expected reply length " << replyLength << " bytes,"
            << "remaining " << numRequestsToSend - 1 << " request\n";

    packet->insertAtBack(payload);
    sendPacket(packet);

    this->videoBuffer->segIndex++;
    numRequestsToSend--;
}

void DashClient::printPacket(Packet *msg)
{
    cout << "========================================"          << endl;
    cout << "[socketDataArrived] Data Arrived Socket "          << endl;
    cout << "Request Number="   << numRequestsToSend            << endl;
    cout << "Total Length="     << msg->getTotalLength()        << endl;
    cout << "Data Length="      << msg->getDataLength()         << endl;
    cout << "Packets Received=" << packetsRcvd                  << endl;
    cout << "Bytes Received="   << this->videoBuffer->bytesRcvd << endl;
    cout << "Resolution="       << this->videoBuffer->res       << endl;
    cout << "Global Time="      << simTime()                    << endl;
}
