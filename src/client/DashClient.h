/**
 * @file DashClient.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#ifndef DASHCLIENT_H_
#define DASHCLIENT_H_




class DashClient
{
public:

	DashClient();
	virtual ~DashClient();
    /**
     * initializes base class-attributes
     *
     * @param stage the init stage
     */
	void initialize(int stage);
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

protected:
	//10 is for if we want to study effects and relations between chunk size and video length
    unsigned short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames */
   	unsigned short int chunkSize; /**< Number of frames in a chunk - */
	unsigned short int gopSize; /**< number of frame available in one GoP (Group of picture)*/
	int bufferSize[10]; /**<number of chunk in buffer */
	int videolenght[10];// length of videos that peer watching them hold in this variable (AGH)
	double bufferMapExchangePeriod; /**<Period in which we push our BufferMap to our neighbors */
    bool isVideoServer; /**<store parameter isVideoServer, if true it is video server */
    bool limitedBuffer;// if it is true = buffer is windowOfIntrest else buffer is film length (AGH)
    unsigned short int Fps; /**< Store parameter Fps, Frame per second of the video*/
    int discontinuityTimeParam;
    short videoAverageRate[10];//
    bool rateControl; /**< if true use rate control mechanism*/
    int maxFrameRequestPerBufferMapExchangePeriod; // for rate control
    int numberOfFrameRequested;//for rate control
    int playbackPoint; /**< store the frame number of the frame that recently send to player module*/
    int recivePoint;
//    PlayingState playingState; /**< when we send first message to player module it become true*/
    bool bufferMapExchangeStart; /**< as first bufferMap received we start to exchange our bufferMap too and it become ture*/
    double startUpBuffering; /** < How much video (second) should buffer in order to play*/
    double measuringTime; /**< the SimTime that we start collecting statistics*/
    unsigned short int receiverSideSchedulingNumber; /**< The number for selecting receiver side scheduling*/
    unsigned short int senderSideSchedulingNumber; /**< The number for selecting sender side scheduling*/
    double averageChunkLength; /**< average of one chunk length*/
    bool schedulingSatisfaction; /**< true if condition is OK for start scheduling*/

};

#endif /* DASHCLIENT_H_ */