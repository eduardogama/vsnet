#include "TCPAdaptiveVideoClientApp.h"

#define MSGKIND_CONNECT     0
#define MSGKIND_SEND        1
#define MSGKIND_VIDEO_PLAY  2

Define_Module(TCPAdaptiveVideoClientApp);

TCPAdaptiveVideoClientApp::TCPAdaptiveVideoClientApp() {
    timeoutMsg = NULL;
}

TCPAdaptiveVideoClientApp::~TCPAdaptiveVideoClientApp() {
    cancelAndDelete(timeoutMsg);
}

void TCPAdaptiveVideoClientApp::initialize(int stage) {
    TcpAppBase::initialize(stage);
    if (stage != 3)
        return;

    // read Adaptive Video (AV) parameters
    const char *str = par("video_packet_size_per_second").stringValue();
    video_packet_size_per_second = cStringTokenizer(str).asIntVector();
    video_buffer_max_length = par("video_buffer_max_length");
    video_duration = par("video_duration");
    manifest_size = par("manifest_size");
    numRequestsToSend = video_duration;
    WATCH(video_buffer);
    video_buffer = 0;
    DASH_buffer_length_signal = registerSignal("DASHBufferLength");
    emit(DASH_buffer_length_signal, video_buffer);
    video_playback_pointer = 0;
    WATCH(video_playback_pointer);
    DASH_playback_pointer = registerSignal("DASHVideoPlaybackPointer");
    emit(DASH_playback_pointer, video_playback_pointer);

    video_current_quality_index = 0;  // start with min quality
    DASH_quality_level_signal = registerSignal("DASHQualityLevel");
    emit(DASH_quality_level_signal, video_current_quality_index);

    video_is_playing = false;
    DASH_video_is_playing_signal = registerSignal("DASHVideoPlaybackStatus");
    //emit(DASH_video_is_playing_signal, video_is_playing);

    WATCH(numRequestsToSend);

    //simtime_t startTime = par("startTime");
    video_startTime = par("video_startTime").doubleValue();
    stopTime = par("stopTime");
    if (stopTime != 0 && stopTime <= video_startTime)
        error("Invalid startTime/stopTime parameters");

    timeoutMsg = new cMessage("timer");
    timeoutMsg->setKind(MSGKIND_CONNECT);
    //scheduleAt(startTime, timeoutMsg);
    EV<< "start time: " << video_startTime << "\n";
    scheduleAt(simTime()+(simtime_t)video_startTime, timeoutMsg);
}

void TCPAdaptiveVideoClientApp::sendRequest() {
    EV<< "sending request, " << numRequestsToSend-1 << " more to go\n";

    // Request length
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
        EV<< "sending manifest request\n";
    }

    // sendPacket(requestLength, replyLength);
}

void TCPAdaptiveVideoClientApp::handleTimer(cMessage *msg) {
    switch (msg->getKind()) {
    case MSGKIND_CONNECT:
        EV<< "starting session\n";
        emit(DASH_video_is_playing_signal, video_is_playing);
        connect(); // active OPEN
        break;

        case MSGKIND_SEND:
        sendRequest();
        // no scheduleAt(): next request will be sent when reply to this one
        // arrives (see socketDataArrived())
        break;

        case MSGKIND_VIDEO_PLAY:
            EV<< "---------------------> Video play event";
            cancelAndDelete(msg);
            video_buffer--;
            emit(DASH_buffer_length_signal, video_buffer);
            video_playback_pointer++;
            emit(DASH_playback_pointer, video_playback_pointer);
            if (video_buffer == 0) {
                video_is_playing = false;

                //cTimestampedValue tmp(simTime() + (simtime_t) 1, (long) video_is_playing);
                //emit(DASH_video_is_playing_signal, &tmp);
                emit(DASH_video_is_playing_signal, video_is_playing);
            }
            if (video_buffer > 0) {
                simtime_t d = simTime() + 1;
                cMessage *videoPlaybackMsg = new cMessage("playback");
                videoPlaybackMsg->setKind(MSGKIND_VIDEO_PLAY);
                scheduleAt(d, videoPlaybackMsg);
                //rescheduleOrDeleteTimer(d, MSGKIND_VIDEO_PLAY);
            }
            if (!video_is_buffering && numRequestsToSend > 0) {
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

void TCPAdaptiveVideoClientApp::socketEstablished(TcpSocket *socket) {
    TcpAppBase::socketEstablished(socket);

    // perform first request
    sendRequest();
}

void TCPAdaptiveVideoClientApp::rescheduleOrDeleteTimer(simtime_t d,
        short int msgKind) {
    cancelEvent (timeoutMsg);

    if (stopTime == 0 || stopTime > d) {
        timeoutMsg->setKind(msgKind);
        scheduleAt(d, timeoutMsg);
    } else {
        delete timeoutMsg;
        timeoutMsg = NULL;
    }
}

void TCPAdaptiveVideoClientApp::socketDataArrived(TcpSocket *socket, Packet *msg,
        bool urgent) {
    TcpAppBase::socketDataArrived(socket, msg, urgent);

    if (!manifestAlreadySent) {
        manifestAlreadySent = true;
        if (timeoutMsg) {
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
        if (can_switch) {
            // Switching algorithm
            simtime_t tSum = 0;
            for (int i = 0; i < packetTimeArrayLength; i++) {
                tSum = tSum + packetTime[i];
            }
            double estimatedBitRate = (packetTimeArrayLength * video_packet_size_per_second[video_current_quality_index]) / tSum;
            EV<< "---------------------> Bit rate estimation:\n";
            EV<< "---------------------> Estimated bit rate = " << estimatedBitRate << "\n";
            int qmax = video_packet_size_per_second.size() -1;
            if (estimatedBitRate > video_packet_size_per_second[std::min(video_current_quality_index + 1, qmax)]) {
                video_current_quality_index = std::min(video_current_quality_index + 1, qmax);
                can_switch = false;
            }
        }
        // the next video fragment will be requested when the buffer gets some space, so nothing to do here.
        return;
    }
    EV<< "---------------------> Buffer=" << video_buffer << "    min= " << video_buffer_min_rebuffering << "\n";
    // Exit rebuffering state and continue the video playback
    if (video_buffer > video_buffer_min_rebuffering || (numRequestsToSend == 0 && video_playback_pointer < video_duration) ) {
        if (!video_is_playing) {
            video_is_playing = true;
            emit(DASH_video_is_playing_signal, video_is_playing);
            simtime_t d = simTime() + 1;   // the +1 represents the time when the video fragment has been consumed and therefore has to be removed from the buffer.
            cMessage *videoPlaybackMsg = new cMessage("playback");
            videoPlaybackMsg->setKind(MSGKIND_VIDEO_PLAY);
            scheduleAt(d, videoPlaybackMsg);
            //rescheduleOrDeleteTimer(d, MSGKIND_VIDEO_PLAY);
        }
    } else {
        video_current_quality_index = std::max(video_current_quality_index - 1, 0);
    }

    if (numRequestsToSend > 0) {
        EV<< "reply arrived\n";

        if (timeoutMsg)
        {
            // Send new request
            simtime_t d = simTime();
            rescheduleOrDeleteTimer(d, MSGKIND_SEND);
        }
    }
    else
    {
        EV << "reply to last request arrived, closing session\n";
        close();
    }
}

void TCPAdaptiveVideoClientApp::socketClosed(TcpSocket *socket) {
    TcpAppBase::socketClosed(socket);

    // Nothing to do here...
}

void TCPAdaptiveVideoClientApp::socketFailure(TcpSocket *socket, int code) {
    TcpAppBase::socketFailure(socket, code);

    // reconnect after a delay
    if (timeoutMsg) {
        simtime_t d = simTime() + (simtime_t) par("reconnectInterval");
        rescheduleOrDeleteTimer(d, MSGKIND_CONNECT);
    }
}
