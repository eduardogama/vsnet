/*
 * CacheService.cpp
 *
 *  Created on: 2 de jun de 2019
 *      Author: eduardo
 */

#include "../services/CacheService.h"

CacheService::CacheService() {
    // TODO Auto-generated constructor stub
}

CacheService::~CacheService() {
    // TODO Auto-generated destructor stub
}

Packet *CacheService::PrepareRequest(TcpSocket *socket, Packet *msg)
{
    std::stringstream req;

    if(true) //if(!msg->HasByteRange())
    {
        req << "GET " << "(segment path)" << " HTTP/1.1\r\n" <<
            "Host: " << socket->getSocketId() << "\r\n\r\n";
    }
    else
    {
        req << "GET " << "segment path" << " HTTP/1.1\r\n" <<
            "Host: " <<  socket->getSocketId() << "\r\n";
//          "Range: bytes=" << msg->StartByte() << "-" << chunk->EndByte() << "\r\n\r\n";
    }

    std::string request = req.str();

    return new Packet();
}


