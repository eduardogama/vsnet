/*
 * DASHApp.cpp
 *
 *  Created on: 20 de mar de 2019
 *      Author: futebol
 */

#include "DASHApp.h"


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
    TcpAppBase::initialize(stage);

    if (stage != 3) return;

    // read Adaptive Video (AV) parameters
    const char *str = par("video_packet_size_per_second").stringValue();
    video_packet_size_per_second = cStringTokenizer(str).asIntVector(); //vector <1000,1500,2000,4000,8000,12000> in kbits

    video_buffer_max_length = par("video_buffer_max_length"); // 10s
    video_duration          = par("video_duration"); // 30s
    manifest_size           = par("manifest_size"); // 100000

    numRequestsToSend = video_duration;

    video_buffer_min_rebuffering = 3; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
    video_buffer                 = 0;
    video_playback_pointer       = 0;
    video_current_quality_index  = 0;  // start with min quality
    video_is_playing             = false;


    video_is_buffering = true;
    manifestAlreadySent = false;

    WATCH(video_buffer);
    WATCH(video_playback_pointer);
    WATCH(numRequestsToSend);

    DASH_buffer_length_signal = registerSignal("DASHBufferLength");
    DASH_playback_pointer     = registerSignal("DASHVideoPlaybackPointer");
    DASH_quality_level_signal = registerSignal("DASHQualityLevel");

    emit(DASH_buffer_length_signal, video_buffer);
    emit(DASH_playback_pointer, video_playback_pointer);
    emit(DASH_quality_level_signal, video_current_quality_index);

    startTime = par("startTime").doubleValue(); // 1s
    stopTime  = par("stopTime"); // 0 means infinity

    if (stopTime != 0 && stopTime <= startTime)
        error("Invalid startTime/stopTime parameters");

    timeoutMsg = new cMessage("timer");
    timeoutMsg->setKind(MSGKIND_CONNECT);

    //scheduleAt(startTime, timeoutMsg);
    EV<< "Start Time: " << startTime << "\n";
    scheduleAt(simTime()+(simtime_t)startTime, timeoutMsg);
}

void DASHApp::sendRequest() {

    EV << "Sending request, " << numRequestsToSend-1 << " more to go\n";

    long requestLength = par("requestLength");
    if (requestLength < 1) requestLength = 1;

    // Reply length
    long replyLength = -1;

    if (manifestAlreadySent) {
        replyLength = video_packet_size_per_second[video_current_quality_index] / 8 * 1000;  // kbits -> bytes

        // Log requested quality
        emit(DASH_quality_level_signal, video_current_quality_index);
        numRequestsToSend--;

        // Switching algoritm
        tLastPacketRequested = simTime();

    } else {
        replyLength = manifest_size;
        EV << "Sending manifest request" << endl;
    }

    //sendPacket(requestLength, replyLength); // STILL REFACTOR
}

void DASHApp::handleTimer(cMessage *msg) {
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            EV << "STARTING SESSION" << endl;

            emit(DASH_video_is_playing_signal, video_is_playing);
            connect();

            break;
        case MSGKIND_SEND:
            EV<< "ENTROU\n";
            sendRequest();

            break;
        case MSGKIND_VIDEO_PLAY:
            EV<< "=> Video play event";

            cancelAndDelete(msg);

            video_buffer--;
            video_playback_pointer++;

            emit(DASH_buffer_length_signal, video_buffer);
            emit(DASH_playback_pointer, video_playback_pointer);

            if (video_buffer == 0) {
                video_is_playing = 0;
                emit(DASH_video_is_playing_signal, video_is_playing);
            }
            if (video_buffer > 0) {
                simtime_t d = simTime() + 1;
                cMessage *videoPlaybackMsg = new cMessage("playback");
                videoPlaybackMsg->setKind(MSGKIND_VIDEO_PLAY);
                scheduleAt(d, videoPlaybackMsg);
            }
            if (!video_is_buffering && numRequestsToSend > 0){
                // Now the buffer has some space
                video_is_buffering = true;
                simtime_t d = simTime();
                rescheduleOrDeleteTimer(d, MSGKIND_SEND);
            }
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void DASHApp::rescheduleOrDeleteTimer(simtime_t d, short int msgKind) {

    cancelEvent(timeoutMsg);

    if(stopTime >= 0) {
        timeoutMsg->setKind(msgKind);
        scheduleAt(d, timeoutMsg);
    } else{
        delete timeoutMsg;
        timeoutMsg = nullptr;
    }
}

void DASHApp::socketEstablished(TcpSocket *socket) {
    TcpAppBase::socketEstablished(socket);

    // perform first request
    sendRequest();
}

void DASHApp::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) {
    TcpAppBase::socketDataArrived(socket, msg, urgent);

    if(!manifestAlreadySent) {
        manifestAlreadySent = true;
        if(timeoutMsg) {
            // Send new request
            simtime_t d = simTime();
            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        }
        return;
    }

    // Switching algorithm
    packetTimePointer = (packetTimePointer + 1) % packetTimeArrayLength;
    packetTime[packetTimePointer] = simTime() - tLastPacketRequested;

    video_buffer++;
    emit(DASH_buffer_length_signal, video_buffer);

    // Update switch timer
    if(!can_switch) {
        switch_timer--;
        if (switch_timer == 0) {
            can_switch = true;
            switch_timer = switch_timeout;
        }
    }

    // Full buffer
    if (video_buffer == video_buffer_max_length) {
        video_is_buffering = false;
        // switch to higher quality (if possible)
        if(can_switch){
            // Switching algorithm
            simtime_t t_sum = 0;

            for (int i = 0; i < packetTimeArrayLength; ++i) {
                t_sum += packetTime[i];
            }

            double estimatedBitRate = (packetTimeArrayLength * video_packet_size_per_second[video_current_quality_index]) / t_sum;
            EV << "---------------------> Bit rate estimation:" << endl;
            EV << "---------------------> Estimated bit rate = " << estimatedBitRate << endl;

            int qmax = video_packet_size_per_second.size() - 1;

            if (estimatedBitRate > video_packet_size_per_second[std::min(video_current_quality_index + 1, qmax)]) {
                video_current_quality_index = std::min(video_current_quality_index + 1, qmax);
                can_switch = false;
            }
        }
        // the next video fragment will be requested when the buffer gets some space, so nothing to do here.
        return;
    }
    EV << "=> Buffer = "
            << video_buffer
            << "|| Min = "
            << video_buffer_min_rebuffering
            << endl;

    // Exit rebuffering state and continue the video playback
    if (video_buffer > video_buffer_min_rebuffering
            || (numRequestsToSend == 0 && video_playback_pointer < video_duration)) {
        if(!video_is_playing){
            video_is_playing = true;
            emit(DASH_video_is_playing_signal, video_is_playing);

            simtime_t d = simTime() + 1;   // the +1 represents the time when the video fragment has been consumed and therefore has to be removed from the buffer.
            cMessage *videoPlaybackMsg = new cMessage("playback");
            videoPlaybackMsg->setKind(MSGKIND_VIDEO_PLAY);
            scheduleAt(d, videoPlaybackMsg);
            //rescheduleOrDeleteTimer(d, MSGKIND_VIDEO_PLAY);
        }
    }
    else {
        video_current_quality_index = std::max(video_current_quality_index - 1, 0);
    }

    if(numRequestsToSend > 0){
        EV << "Reply arrived" << endl;
        if(timeoutMsg){
            //Send new request
            simtime_t d = simTime();
            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        }
    }
    else {
        EV << "Reply last request arrived, closing session" << endl;
        close();
    }
}

void DASHApp::socketClosed(TcpSocket *socket) {
    TcpAppBase::socketClosed(socket);

    // Nothing to do here...
}

void DASHApp::socketFailure(TcpSocket *socket, int code) {
    TcpAppBase::socketFailure(socket, code);
    // TODO
}

void DASHApp::handleStartOperation(LifecycleOperation *operation)
{
    // TODO
}

void DASHApp::handleStopOperation(LifecycleOperation *operation)
{
    // TODO
}

void DASHApp::handleCrashOperation(LifecycleOperation *operation)
{
    // TODO
}
