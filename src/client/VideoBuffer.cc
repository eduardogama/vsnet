#include "VideoBuffer.h"

VideoBuffer::VideoBuffer()
{
    this->videostream = new list<Segment>();
}

VideoBuffer::~VideoBuffer()
{
    delete videostream;
}

double VideoBuffer::estimatedBitRate()
{
    return 0.0;
}

bool VideoBuffer::isReady()
{
    return this->videostream->size() >= this->minPlayBack;
}

bool VideoBuffer::isEmpty()
{
    return this->videostream->empty();
}

Segment &VideoBuffer::playingBackSeg()
{
    return this->videostream->front();
}

void VideoBuffer::addSegment(Segment &s)
{
    this->videostream->push_back(s);
}

void VideoBuffer::removeSegment(Segment &s)
{
}

void VideoBuffer::removeFirstSegment()
{
    this->videostream->pop_front();
}

void VideoBuffer::removeLastSegment()
{
    this->videostream->pop_back();
}

