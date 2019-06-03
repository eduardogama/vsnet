#include "VideoBuffer.h"

VideoBuffer::VideoBuffer()
{
}

VideoBuffer::~VideoBuffer()
{
}

double VideoBuffer::estimatedBitRate()
{
    return 0.0;
}

void VideoBuffer::addSegment(Segment &s)
{
    this->segmentBuffer.push_back(s);
}
