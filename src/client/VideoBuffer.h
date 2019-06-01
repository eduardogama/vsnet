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
		int bytesRcvd;
        int segmentSize; /**< number of frames available in a chunk*/
		int interactiveChunkStart;
		int seekChuncknumbers;

		int numRequestsToSend;
		simtime_t reqtime;

		int lastSetChunk; /**< last chunk that recently set*/
		int lastSetFrame; /**< last frame that recently set*/
		int segmentframes; /**< Number of frames per Group of Picture */
		vector<Segment> segmentBuffer; /**< video buffer that keeps some number of chunks*/

};

#endif /* VIDEOBUFFER_H_ */
