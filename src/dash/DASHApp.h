/*
 * DASHApp.h
 *
 *  Created on: 20 de mar de 2019
 *      Author: futebol
 */

#ifndef DASH_DASHAPP_H_
#define DASH_DASHAPP_H_

#include <algorithm>

#include "inet/common/INETDefs.h"
#include "inet/applications/tcpapp/TcpAppBase.h"


namespace inet {

class DASHApp: public  TcpAppBase {
    public:
        DASHApp();
        virtual ~DASHApp();

    protected:
        /** Redefined **/
        virtual void initialize(int stage) override;
        /** Redefined **/
        virtual void handleMessage(cMessage);
        /** Redefined. **/
        void handleTimer(cMessage *msg) override;
        /** Redefined. **/
        virtual void socketEstablished(TcpSocket *socket) override;
        /** Redefined. **/
        virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;

        /** Redefined to start another session after a delay (currently not used). **/
        virtual void socketClosed(TcpSocket *socket) override;

        /** Redefined to reconnect after a delay. **/
        virtual void socketFailure(TcpSocket *socket, int code) override;

        /** Utility: sends a request to the server **/
        virtual void sendRequest();

        /**
         * Utility: cancel msgTimer and if d is smaller than stopTime, then schedule it to d,
         * otherwise delete msgTimer
         **/
        virtual void rescheduleOrDeleteTimer(simtime_t d, short int msgKind);

        virtual void handleStartOperation(LifecycleOperation *operation) override;
        virtual void handleStopOperation(LifecycleOperation *operation) override;
        virtual void handleCrashOperation(LifecycleOperation *operation) override;

    protected:
        // Adaptive Video (AV) parameters
        std::vector<int> video_packet_size_per_second;
        int video_buffer_max_length;
        int video_duration;
        int numRequestsToSend; // requests to send in this session. Each request = 1s of video
        int video_buffer; // current lenght of the buffer in seconds
        simsignal_t DASH_buffer_length_signal;
        int video_current_quality_index; // requested quality
        simsignal_t DASH_quality_level_signal;
        bool video_is_playing;
        simsignal_t DASH_video_is_playing_signal;
        int video_playback_pointer;
        simsignal_t DASH_playback_pointer;
        bool video_is_buffering = true;
        int video_buffer_min_rebuffering = 3; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
        int manifest_size;
        bool manifestAlreadySent = false;
        double video_startTime;

        cMessage *timeoutMsg;
        simtime_t stopTime;

        // Flags to avoid multiple quality switches when the buffer is at full capacity
        bool can_switch = true;
        int switch_timeout = 5;
        int switch_timer = switch_timeout;
        // Enhanced switch algorithm (estimate available bit rate before switching to higher quality level)
        int packetTimeArrayLength = 5;
        simtime_t packetTime[5];
        int packetTimePointer = 0;
        simtime_t tLastPacketRequested;

};

}


#endif /* DASH_DASHAPP_H_ */
