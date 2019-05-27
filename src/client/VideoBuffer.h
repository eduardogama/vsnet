#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include "Segment.h"

class VideoBuffer
{
	protected:

		short int numOfBFrame; /**<Number of B frames between 'I' and 'P' or between two 'P' frames; */
		int bufferSize; /**< number of chunk available in the buffer*/
		short int chunkSize; /**< number of frames available in a chunk*/
	public:
		int interactiveChunkStart;
		int seekChuncknumbers;

		int lastSetChunk; /**< last chunk that recently set*/
		int lastSetFrame; /**< last frame that recently set*/
		short int gopSize; /**< Number of frames per Group of Picture */
		Segment* segmentBuffer; /**< video buffer that keeps some number of chunks*/

};

#endif /* VIDEOBUFFER_H_ */
