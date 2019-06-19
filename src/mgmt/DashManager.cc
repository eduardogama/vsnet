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

#include "DashManager.h"


DashManager::DashManager(Segment *segment, MPDRequestHandler *mpd) : segment(segment), mpd(mpd)
{
    // TODO Auto-generated constructor stub
    this->firstReq = true;
    this->segIndex = 0;
}

DashManager::DashManager() {
    // TODO Auto-generated constructor stub
    this->firstReq = true;
    this->segIndex = 0;
}

DashManager::~DashManager() {
    // TODO Auto-generated destructor stub
}

Segment* DashManager::BitRateAssigment(VideoBuffer *v) {
    int index=0;
    double t_sum=0;
    simtime_t t_time = 0;

//    std::cout << "Video Stream Size=" << v->videostream->size() << std::endl;

    for (std::list<Segment>::iterator it = v->videostream->begin(); it != v->videostream->end() && index < 20; ++it) {
        t_sum  += it->getSegmentSize();
        t_time += it->getEndTime() - it->getStartTime();
//        std::cout << "Segment Size=" << it->getSegmentSize() << std::endl;
//        std::cout << "StartTime=" << it->getStartTime() << " EndTime=" << it->getEndTime() << std::endl;
        index++;
    }

    double throughput = t_sum / t_time;

    simtime_t maxvalue   = 0;
    std::string res;

//    std::cout << "Throughput=" << throughput << std::endl;
    for (std::vector<std::string>::iterator it = this->qualities->begin(); it != this->qualities->end(); ++it) {

        Representation &rep = (*this->representation)[*it];
        double bw = rep.segments[this->segIndex].mediaRange;

        simtime_t estimateBR = bw/throughput;

        if((estimateBR < this->mpd->getMinBufferTime()) && (maxvalue < estimateBR)){
//            std::cout << "Resolution=" << *it  << " " << rep.segments[this->segIndex].mediaRange
//                      << " " << rep.segments[this->segIndex].media << " " << estimateBR << std::endl;
            res = *it;
            maxvalue = estimateBR;
        }
    }

    Segment *seg = new Segment();

    simtime_t time = simTime();
    int size = (*this->representation)[res].segments[this->segIndex].mediaRange;

    seg->setStartTime(simTime());
    seg->setSegmentSize(size);
    seg->setQuality(res);
    seg->setSegmentNumber(this->segIndex);

    this->segIndex += 1;

    return seg;
}

Segment *DashManager::LowRepresentation()
{
    this->segIndex += 1;
    return this->mpd->LowRepresentation();
}

MPDRequestHandler* DashManager::getMpd() {
    return this->mpd;
}

void DashManager::setMpd(MPDRequestHandler* mpd) {
    this->mpd = mpd;
}

Segment* DashManager::getSegment() {
    return segment;
}

void DashManager::setSegment(Segment* segment) {
    this->segment = segment;
}

vector<std::string>*& DashManager::getQualities() {
    return qualities;
}

void DashManager::setQualities(vector<std::string>* qualities) {
    this->qualities = qualities;
}

map<std::string, Representation>*& DashManager::getRepresentation() {
    return representation;
}

void DashManager::setRepresentation(map<std::string, Representation>* representation) {
    this->representation = representation;
}
