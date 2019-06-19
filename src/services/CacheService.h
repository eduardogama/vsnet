/*
 * CacheService.h
 *
 *  Created on: 2 de jun de 2019
 *      Author: eduardo
 */

#ifndef SERVICES_CACHESERVICE_H_
#define SERVICES_CACHESERVICE_H_

#include <omnetpp/platdep/platdefs.h>
#include <omnetpp/simtime_t.h>
#include <map>
#include <list>

#include "inet/applications/tcpapp/TcpGenericServerApp.h"
#include "inet/common/packet/ChunkQueue.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "client/Segment.h"


using namespace std;
using namespace inet;

struct VideoStreamDash
{
    cMessage *timer = nullptr;    // self timer msg
    L3Address clientAddr;    // client address
    int clientPort = -1;    // client TCP port
    long videoSize = 0;    // total size of video
    long bytesLeft = 0;    // bytes left to transmit
    long numPkSent = 0;    // number of packets sent
};

typedef std::map<long int, VideoStreamDash> VideoStreamMap;


class CacheService: public TcpGenericServerApp {
    public:
        CacheService();
        virtual ~CacheService();

        virtual void initialize(int stage) override;

        virtual int numInitStages() const override { return NUM_INIT_STAGES; }

        virtual void handleMessage(cMessage *msg) override;

        Packet *PrepareRequest(TcpSocket *socket, Packet *msg);

    protected:
        TcpSocket socket;
        simtime_t delay;
        simtime_t maxMsgDelay;

        long msgsRcvd;
        long msgsSent;
        long bytesRcvd;
        long bytesSent;

        map<int, ChunkQueue> socketQueue;

        map<string, list<Segment>> videoCache;

        VideoStreamMap streams;
//        std::vector<Segment> segmentCache;

};

#endif /* SERVICES_CACHESERVICE_H_ */
