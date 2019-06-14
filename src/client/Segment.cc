//
// Copyright (C) 2012 Tarbiat Modares University All Rights Reserved.
// http://ece.modares.ac.ir/mnl/
// This system was designed and developed at the DML(Digital Media Lab http://dml.ir/)
// under supervision of Dr. Behzad Akbari (http://www.modares.ac.ir/ece/b.akbari)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

/**
 * @file Segment.h
 * @author Abdollah Ghaffari Sheshjavani(AGH), Behnam Ahmadifar, Yasser Seyyedi
 */

#include "Segment.h"

Segment::Segment() {
    segmentNumber = -1;
}

Segment::Segment(int frameRate, int width, int height, int segmentNumber, int segmentSize, simtime_t startTime, simtime_t endTime)
{
    this->startTime     = startTime;
    this->endTime       = endTime;
    this->frameRate     = frameRate;
    this->width         = width;
    this->height        = height;
    this->segmentNumber = segmentNumber;
    this->segmentSize   = segmentSize;
}

Segment::~Segment() {
}

int Segment::getSegmentSize() {
    return segmentSize;
}

void Segment::setValues(int SegmentSize, int frameRate, int width, int height) {
    this->segmentSize = SegmentSize;
    this->frameRate = frameRate;
    this->width = width;
    this->height = height;

    this->startTime = simTime();
}

bool Segment::isComplete() {
    for (int i = 0; i < segmentSize; i++)
        if (!segment[i].isSet())
            return false;
    return true;
}
void Segment::setFilmNumber(int FilmNumber) {
    filmNumber = FilmNumber;
}
void Segment::setSegmentNumber(int SegmentNumber) {
    segmentNumber = SegmentNumber;
}
void Segment::setFrame(bool isServer, VideoFrame vFrame) {
    int ExtractedFrameNum = vFrame.getFrameNumber() % segmentSize;
    segment[ExtractedFrameNum].setFrame(vFrame);
}

int Segment::getSegmentByteLength() {
    int totalSize = 0;
    if (isComplete()) {
        for (int i = 0; i < segmentSize; i++)
            totalSize += segment[i].getFrameLength();
        return totalSize;
    } else
        return -1;
}

std::ostream& operator<<(std::ostream& os, const Segment& c) {
    os << c.segmentNumber << ": ";
    for (int i = 0; i < c.segmentSize; i++)
        os << c.segment[i] << " ";
    return os;
}
void Segment::setHopCout(int HopCount) {
    hopCount = HopCount;
}
/*double Segment::getCreationTime()
 {
 return segment[segmentSize -1].getCreationTime();
 }*/
int Segment::getLastFrameNo() {
    return segment[segmentSize - 1].getFrameNumber();
}
int Segment::getLateArrivalLossSize(int playBackPoint) {
    int totalSize = 0;
    for (int i = 0; i < segmentSize; i++)
        if (segment[i].getFrameNumber() > playBackPoint)
            totalSize += segment[i].getFrameLength();
    return totalSize;
}

const simtime_t& Segment::getDuration() const {
    return duration;
}

void Segment::setDuration(const simtime_t& duration) {
    this->duration = duration;
}

const simtime_t& Segment::getEndTime() const {
    return this->endTime;
}

void Segment::setEndTime(const simtime_t& endTime) {
    this->endTime = endTime;
}

int Segment::getFrameRate() const {
    return frameRate;
}

void Segment::setFrameRate(int frameRate) {
    this->frameRate = frameRate;
}

void Segment::setSegmentSize(int segmentSize) {
    this->segmentSize = segmentSize;
}

const simtime_t& Segment::getStartTime() const {
    return startTime;
}

void Segment::setStartTime(const simtime_t& startTime) {
    this->startTime = startTime;
}

const std::string& Segment::getQuality() const {
    return quality;
}

void Segment::setQuality(const std::string& quality) {
    this->quality = quality;
}
