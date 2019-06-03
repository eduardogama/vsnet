#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include <vector>

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

	    int playbbackPtr;
	    int minPlayBack;
	    int maxBuffer;
	    bool isPlaying;

};

#endif /* VIDEOBUFFER_H_ */
