/*
 * CacheService.h
 *
 *  Created on: 2 de jun de 2019
 *      Author: eduardo
 */

#ifndef WEBSERVER_CACHESERVICE_H_
#define WEBSERVER_CACHESERVICE_H_

#include <vector>
#include <string>

#include "inet/applications/httptools/server/HttpServer.h"

#include "client/Segment.h"


using namespace std;
using namespace inet;
using namespace httptools;

class CacheService: public HttpServer {
    public:
        CacheService();
        virtual ~CacheService();

        Packet *PrepareRequest(TcpSocket *socket, Packet *msg);

    protected:
        vector<Segment> segmentCache;
};

#endif /* WEBSERVER_CACHESERVICE_H_ */
