/**
 * @file DashClient.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#ifndef DASHCLIENT_H_
#define DASHCLIENT_H_

#include <string>
#include <vector>
#include <iostream>

#include "inet/applications/tcpapp/TcpBasicClientApp.h"
#include "inet/common/packet/Packet.h"



#include <omnetpp/clistener.h>
#include <omnetpp/platdep/platdefs.h>
#include <omnetpp/simtime_t.h>
#include <map>

#include "inet/common/packet/ChunkQueue.h"
#include "inet/common/packet/ReassemblyBuffer.h"

#include "Segment.h"
#include "VideoBuffer.h"
#include "MPDRequestHandler.h"

#include "mgmt/DashManager.h"


using namespace inet;  // namespace inet

typedef struct DashPlayback_t{
    std::string title;
    std::string mimeType;
    std::string vcodec;
    int duration;
    int maxFrameRate;
    int playbbackPtr;
    bool isPlaying;

} DashPlayback;

class DashClient : public TcpBasicClientApp
{
    public:
        DashClient();
        ~DashClient();

        /**
         * initializes base class-attributes
         *
         * @param stage the init stage
         */
        void initialize(int stage) override;

        /**
         *
         */
        void decodePacket(Packet *vp);

        /**
         * handleMessage method description
         */
//      void handleMessage(cMessage *msg) override;

        /**
         * Finish method description
         */
//      virtual void finish() override;

        /**
         * initializes config in MPD file
         */
        void printPacket(Packet *msg);

    /*    virtual void finishApp();*/
    /*    virtual void handleTimerEvent(cMessage* msg);*/
    /*    virtual void handleLowerMessage(cMessage* msg);*/
    /*    virtual void handleUpperMessage(cMessage* msg);*/

        /**
         * delete a frame number from sendframes vector (after timeout)
         * @param framenum frame Number to delete
         * @param sendFrames vector of frame that is already send
         */
    /*  virtual void deleteElement(int frameNum, std::vector <requestedFrames>& sendFrames);*/
        /**
         * broadcast buffermap to all the node neighbor
         */
    /*  virtual void bufferMapExchange();*/
        /**
         * check if we whether enough frames eqaul to startup buffering to play
         */
    /*  virtual void checkForPlaying();*/

        void prepareRequest();

        /** Redefined. **/
        virtual void sendRequest() override;

        virtual void rescheduleOrDeleteTimer(simtime_t d, short int msgKind) override;

        virtual void handleTimer(cMessage *msg) override;

        void Connect();

        /** Redefined. **/
        virtual void socketEstablished(TcpSocket *socket) override;

        /** Redefined. **/
        virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;

        /** Redefined to start another session after a delay (currently not used). **/
        virtual void socketClosed(TcpSocket *socket) override;

        /** Redefined to reconnect after a delay. **/
        virtual void socketFailure(TcpSocket *socket, int code) override;

        virtual void handleStartOperation(LifecycleOperation *operation) override;
        virtual void handleStopOperation(LifecycleOperation *operation) override;
        virtual void handleCrashOperation(LifecycleOperation *operation) override;

    protected:
        DashManager       *dashmanager;

        Representation    *representation;
        Segment           *c_segment;

        VideoBuffer       *videoBuffer;

        DashPlayback      *dashplayback;

        MPDRequestHandler *mpd;

        // Adaptive Video (AV) parameters
        std::vector<int> video_packet_size_per_second;

        std::map<int, ChunkQueue> socketQueue;

        ChunkQueue queue;

//        ReassemblyBuffer buffer; // stores received data

        std::string connectAddress;

//        int video_buffer_max_length;
//        int video_duration;
//        int manifest_size;

//        int video_buffer_min_rebuffering; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
//        int video_buffer; // current lenght of the buffer in seconds
//        int video_playback_pointer;
//        int video_current_quality_index; // requested quality
//        bool video_is_playing;

//        bool video_is_buffering = true;
//        bool manifestAlreadySent = false;

//        simsignal_t DASH_buffer_length_signal;
//        simsignal_t DASH_quality_level_signal;
//        simsignal_t DASH_video_is_playing_signal;
//        simsignal_t DASH_playback_pointer;

        simsignal_t DASH_seg_cmplt;
        simsignal_t DASH_video_is_playing;
        simsignal_t DASH_buffer_length;
};

#endif /* DASHCLIENT_H_ */
