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

#ifndef MGMT_DASHMANAGER_H_
#define MGMT_DASHMANAGER_H_

#include "../client/MPDRequestHandler.h"

class DashManager {
    public:

        DashManager(Representation *representation, Segment *segment, MPDRequestHandler *mpd);

        DashManager();
        virtual ~DashManager();
        /*
         *
         */
        Segment *BitRateAssigment();
        /*
         *
         */
        Segment *LowRepresentation();
        /*
         *
         */
        MPDRequestHandler* getMpd();
        /*
         *
         */
        void setMpd(MPDRequestHandler* mpd);
        /*
         *
         */
        Representation* getRepresentation();
        /*
         *
         */
        void setRepresentation(Representation* representation);
        /*
         *
         */
        Segment* getSegment();
        /*
         *
         */
        void setSegment(Segment* segment);

    protected:
        Representation *representation;
        Segment *segment;
        MPDRequestHandler *mpd;

        bool firstReq;
};

#endif /* MGMT_DASHMANAGER_H_ */
