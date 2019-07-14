/*
 * CacheService.h
 *
 *  Created on: 2 de jun de 2019
 *      Author: eduardo
 */

#ifndef SERVICES_CACHESERVICE_H_
#define SERVICES_CACHESERVICE_H_

#include <map>
#include <list>

#include "inet/applications/tcpapp/TcpGenericServerApp.h"
#include "inet/common/packet/ChunkQueue.h"
#include "inet/common/socket/SocketMap.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "client/Segment.h"

#include "mgmt/DashManager.h"


using namespace std;
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

class CacheServiceBase;

class CacheService: public TcpGenericServerApp {
    public:
        CacheService();
        ~CacheService();

        virtual void initialize(int stage) override;

        virtual void handleMessage(cMessage *msg) override;

        Packet *PrepareRequest(TcpSocket *socket, Packet *msg);

    private:
        void storeSegmentVideo(const char* movie, Segment *seg);
    protected:
        TcpSocket socket;

        TcpSocket serverSocket;
        SocketMap socketMap;
        typedef std::set<CacheServiceBase *> ThreadSet;

        ThreadSet threadSet;

        simtime_t delay;
        simtime_t maxMsgDelay;

        long msgsRcvd;
        long msgsSent;
        long bytesRcvd;
        long bytesSent;

        map<int, ChunkQueue> socketQueue;

        VideoStreamMap streams;
        //vector<Segment> segmentCache;

        map<int, NodeType> nodeMap;
};

class CacheServiceBase
{
    public:
        void insertSegment(std::string path_seg);

    protected:
        map<string, list<Segment>> videoCache;

};


#endif /* SERVICES_CACHESERVICE_H_ */
