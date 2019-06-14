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
 * @author Abdollah Ghaffari Sheshjavani (AGH),Behnam Ahmadifar, Yasser Seyyedi
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include "inet/common/INETDefs.h"

#include "VideoFrame.h"


using namespace inet;

class Segment
{
    protected:
        simtime_t startTime;
        simtime_t endTime;

        simtime_t duration;

        std::string quality;

        int frameRate;
        int width;
        int height;
        int segmentNumber; /**< The segment number of the created object */
        short int filmNumber;  // Film number of segment- for vod -(AGH)
        int segmentSize; /**< Number of frames in the segment object*/
        short int hopCount; /**< Hop count of the segment that traverse through the network*/
        //double creationTime; /**< Time that the segment is created. It is use for calculating end to end delay*/
    public:
        VideoFrame* segment; /**< array that keeps video frames as a segment*/
        /**
         * This function is used in order to initialize the segment class
         *
         * @param SegmentSize number of frame available in this segment
         */
        void setValues(int SegmentSize, int frameRate, int width, int height);
        /**
         * Base class constructor
         */
        Segment();
        /**
         * Base class constructor
         */
        Segment(int frameRate, int width, int height, int segmentNumber, int segmentSize, simtime_t startTime=0, simtime_t endTime=0);
        /**
         * Base class distructor
         */
        ~Segment();

        /**
         * @return an integer, it contains segment size
         */
        int getSegmentSize();
        /**
         * set a frame in segment array acording to its number
         *
         * @param VideoFrame a frame to place in this segment
         */
        void setFrame(bool isServer,VideoFrame vFrame);
        /**
         * check whether this segment fill with its frames
         *
         * @return boolean true if it is contained all frames
         */
        bool isComplete();
        /**
         * set the Film id of this segment-AGH
         *
        * @param FilmNumber is id of film-AGH
        */
        void setFilmNumber(int FilmNumber);

        /**
         * set the number of this segment
         *
         * @param SegmentNumber the number to set for segment number
         */
        void setSegmentNumber(int SegmentNumber);

        /* * set the Film id of this segment-AGH
         *
        * @param FilmNumber is id of film-AGH
        */
         int getFilmNumber(){return filmNumber;};
        /**
         * get the segment number of this segment
         *
         * @return integer the segment number
         */
         int getSegmentNumber(){return segmentNumber;}
        /**
         * get the byte size of the segment based on contained frames and other variables
         *
         * @return integer byte size of this segment
         */
        int getSegmentByteLength();
        /**
         * get the value of the hop that the segment traverse
         *
         * @return integer hop count number
         */
        int getHopCount(){return hopCount;}
        /**
         * for geting the time of creating this segment
         * @return double creation time value
         */
        //double getCreationTime();
        /**
         * set hop count number
         *
         * @param HopCount to be set
         */
        void setHopCout(int HopCount);
        /**
         * standard output stream for Segment,
         * gives out all contained frames
         *
         * @param os the ostream
         * @param c the Segment
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const Segment& c);
        /**
         * gives the last frame number that is insert in this segment
         *
         * @return integer las frame numbers
         */
        int getLastFrameNo();
        /**
         * calculate the size of frame that will lost due to late arrival
         *
         * @param playbackPoint frame number that is currently is playing
         * @return integer size of late arrival frames
         */
        int getLateArrivalLossSize(int playBackPoint);
        /**
         *
         */
        const simtime_t& getDuration() const;
        /**
         *
         */
        void setDuration(const simtime_t& duration);

        /**
         *
         */
        const simtime_t& getEndTime() const;

        /**
         * This function is used when the transmission segment is completed
         *
         * @param endtime the end time of segment
         */
        void setEndTime(const simtime_t& endTime);

        /**
         *
         */
        int getFrameRate() const;

        /**
         *
         */
        void setFrameRate(int frameRate);

        /**
         *
         */
        void setSegmentSize(int segmentSize);

        /**
         *
         */
        const simtime_t& getStartTime() const;

        /**
         *
         */
        void setStartTime(const simtime_t& startTime);

        const std::string& getQuality() const;

        void setQuality(const std::string& quality);
};
#endif /* CHUNK_H_ */
