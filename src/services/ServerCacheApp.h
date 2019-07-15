//
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_SERVERCACHEAPP_H
#define __INET_SERVERCACHEAPP_H

#include "inet/applications/tcpapp/TcpServerHostApp.h"

#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

#include "client/Segment.h"
#include "mgmt/DashManager.h"


using namespace inet;

struct VideoStreamDash
{
    cMessage *timer = nullptr;    // self timer msg
    long videoSize = 0;    // total size of video
    long numPkSent = 0;    // number of packets sent
    map<int, Segment> videos;
};

typedef map<unsigned int, Segment> VideoSegments;
typedef map<const char*, VideoSegments> VideoStreamMap;


class ServerCacheApp : public TcpServerHostApp
{
    protected:
        long bytesRcvd = 0;
        VideoStreamMap streams;

        map<int, NodeType> nodeMap;

    protected:
        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void finish() override;
        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void refreshDisplay() const override;

        virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override;

        virtual void handleStartOperation(LifecycleOperation *operation) override;
    public:
        ServerCacheApp();
        ~ServerCacheApp();

    private:
        void storeSegmentVideo(const char* movie, Segment *seg);

    friend class ServerCacheThread;
};

class ServerCacheThread : public TcpServerThreadBase
{
    protected:
        long bytesRcvd;
        ServerCacheApp *cacheAppModule = nullptr;

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void refreshDisplay() const override;

        //TcpServerThreadBase:
        /**
        * Called when connection is established.
        */
        virtual void established() override;

        /*
        * Called when a data packet arrives.
        */
        virtual void dataArrived(Packet *msg, bool urgent) override;

        /*
        * Called when a timer (scheduled via scheduleAt()) expires.
        */
        virtual void timerExpired(cMessage *timer) override { throw cRuntimeError("Model error: unknown timer message arrived"); }

        virtual void init(TcpServerHostApp *hostmodule, TcpSocket *socket) override { TcpServerThreadBase::init(hostmodule, socket); cacheAppModule = check_and_cast<ServerCacheApp *>(hostmod); }

};

#endif // ifndef __INET_SERVERCACHEAPP_H

