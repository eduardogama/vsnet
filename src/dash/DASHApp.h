/*
 * DASHApp.h
 *
 *  Created on: 20 de mar de 2019
 *      Author: futebol
 */

#ifndef DASH_DASHAPP_H_
#define DASH_DASHAPP_H_

#include <algorithm>

#include "inet/common/INETDefs.h"
#include "inet/applications/httptools/common/HttpNodeBase.h"

#include "inet/common/packet/ChunkQueue.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/socket/SocketMap.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

#include "client/Segment.h"
#include "client/VideoBuffer.h"
#include "client/MPDRequestHandler.h"


using namespace inet;
using namespace httptools;

class DASHApp: public HttpNodeBase, public TcpSocket::ICallback
{
    public:
        DASHApp();
        virtual ~DASHApp();

    protected:
        /** Redefined **/
        void initialize(int stage) override;

        /** Redefined **/
        void handleMessage(cMessage *msg) override;

        /** Redefined. **/
        void prepareRequest();

        /** Redefined. **/
        void sendRequest();

        /** Redefined. **/
        virtual void socketEstablished(TcpSocket *socket) override;

        /** Redefined. **/
        virtual void socketPeerClosed(TcpSocket *socket) override;

        /** Redefined. **/
        virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;

        /** Redefined. **/
        virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override;

        /** Redefined to start another session after a delay (currently not used). **/
        virtual void socketClosed(TcpSocket *socket) override;

        /** Redefined to reconnect after a delay. **/
        virtual void socketFailure(TcpSocket *socket, int code) override;

        /** Redefined. **/
        virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override;

        /** Redefined. **/
        virtual void socketDeleted(TcpSocket *socket) override;

    protected:
        Representation *representation;
        Segment *currentSegment;
        VideoBuffer *videoBuffer;

        MPDRequestHandler *mpd;

        simsignal_t DASH_seg_cmplt;

        cMessage *timeoutMsg = nullptr;
        bool earlySend = false;    // if true, don't wait with sendRequest() until established()
        int numRequestsToSend = 0;    // requests to send in this session
        simtime_t startTime;
        simtime_t stopTime;
};


#endif /* DASH_DASHAPP_H_ */
