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


#include "VideoBuffer.h"
#include "MPDRequestHandler.h"

#include "Segment.h"


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
//	    void handleMessage(cMessage *msg) override;

	    /**
         * Finish method description
         */
//	    virtual void finish() override;

		/**
		 * initializes config in MPD file
		 */
		void ReadMPD();
		
	/*    virtual void finishApp();*/
	/*    virtual void handleTimerEvent(cMessage* msg);*/
	/*    virtual void handleLowerMessage(cMessage* msg);*/
	/*    virtual void handleUpperMessage(cMessage* msg);*/

		/**
		 * delete a frame number from sendframes vector (after timeout)
		 * @param framenum frame Number to delete
		 * @param sendFrames vector of frame that is already send
		 */
	/*	virtual void deleteElement(int frameNum, std::vector <requestedFrames>& sendFrames);*/
		/**
		 * broadcast buffermap to all the node neighbor
		 */
	/*	virtual void bufferMapExchange();*/
		/**
		 * check if we whether enough frames eqaul to startup buffering to play
		 */
	/*	virtual void checkForPlaying();*/

        void prepareRequest();

		/** Redefined. **/
		virtual void sendRequest() override;

		virtual void rescheduleOrDeleteTimer(simtime_t d, short int msgKind) override;

        virtual void handleTimer(cMessage *msg) override;

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

        Segment *currentSegment;
        VideoBuffer *videoBuffer;

		DashPlayback *dashplayback;
		
		MPDRequestHandler *mpd;

        // Adaptive Video (AV) parameters
        std::vector<int> video_packet_size_per_second;

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

//		std::vector <int> requestedChunks;

		//10 is for if we want to study effects and relations between chunk size and video length
//		unsigned short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames */
//	   	unsigned short int chunkSize; /**< Number of frames in a chunk - */
//		int bufferSize[10]; /**<number of chunk in buffer */
//		int videolenght[10];// length of videos that peer watching them hold in this variable (AGH)
//		double bufferMapExchangePeriod; /**<Period in which we push our BufferMap to our neighbors */
//		bool isVideoServer; /**<store parameter isVideoServer, if true it is video server */
//		bool limitedBuffer;// if it is true = buffer is windowOfIntrest else buffer is film length (AGH)
//		unsigned short int Fps; /**< Store parameter Fps, Frame per second of the video*/
//		int discontinuityTimeParam;
//		short videoAverageRate[10];//
//		bool rateControl; /**< if true use rate control mechanism*/
//		int maxFrameRequestPerBufferMapExchangePeriod; // for rate control
//		int numberOfFrameRequested;//for rate control
//		int playbackPoint; /**< store the frame number of the frame that recently send to player module*/
//		int recivePoint;
//	//    PlayingState playingState; /**< when we send first message to player module it become true*/
//		bool bufferMapExchangeStart; /**< as first bufferMap received we start to exchange our bufferMap too and it become ture*/
//		double startUpBuffering; /** < How much video (second) should buffer in order to play*/
//		double measuringTime; /**< the SimTime that we start collecting statistics*/
//		unsigned short int receiverSideSchedulingNumber; /**< The number for selecting receiver side scheduling*/
//		unsigned short int senderSideSchedulingNumber; /**< The number for selecting sender side scheduling*/
//		double averageChunkLength; /**< average of one chunk length*/
//		bool schedulingSatisfaction; /**< true if condition is OK for start scheduling*/

};

#endif /* DASHCLIENT_H_ */
