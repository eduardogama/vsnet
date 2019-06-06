#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include <vector>
#include <list>

#include "inet/common/INETDefs.h"

#include "Segment.h"


using namespace std;
using namespace inet;

class VideoBuffer
{
	protected:
		short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames; */
		int bufferSize; /**< number of chunk available in the buffer*/
	public:
		int segIndex;
		int bytesRcvd;
        int segmentSize; /**< number of frames available in a chunk*/

		int numRequestsToSend;
		simtime_t reqtime;

		int segmentframes; /**< Number of frames per Group of Picture */
		vector<Segment> segmentBuffer; /**< video buffer that keeps some number of chunks*/

		list<Segment> *videostream;

	    int playbbackPtr;
	    int minPlayBack;
	    int maxBuffer;
	    bool isPlaying;

	public:

	    VideoBuffer();
	    virtual ~VideoBuffer();

	    /**
         * Estimate bandwith according to avg. throughput
         *
         * @return integer bitrate
         */
        double estimatedBitRate();

        /**
         * Add segment in buffer
         */
        bool isReady();

        /**
         * Check buffer size, if buffer is empty then true otherwise false
         */
        bool isEmpty();

        /**
         * Check buffer size, if buffer is empty then true otherwise false
         */
        Segment &playingBackSeg();

        /**
         * Add segment in buffer
         */
        void addSegment(Segment &s);

        /**
         * Remove segment in buffer
         */
        void removeSegment(Segment &s);

        /**
         * Remove first segment in buffer
         */
        void removeFirstSegment();

        /**
         * Add last segment in buffer
         */
        void removeLastSegment();
};

#endif /* VIDEOBUFFER_H_ */
