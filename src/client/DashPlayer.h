/*
 * DashPlayer.h
 *
 *  Created on: 3 de jun de 2019
 *      Author: futebol
 */

#ifndef CLIENT_DASHPLAYER_H_
#define CLIENT_DASHPLAYER_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"


using namespace inet;

enum
{
    MPEG_PLAYER_PAUSED, MPEG_PLAYER_PLAYING, MPEG_PLAYER_NOT_STARTED, MPEG_PLAYER_DONE
};

class DashPlayer : public ApplicationBase{
    public:
        DashPlayer();
        virtual ~DashPlayer();

        /**
         *
         */
        virtual void initialize(int stage) override;
        /**
         *
         */
        virtual void handleTimer(cMessage *msg);
        /**
         *
         */
        void Play();
        /**
         *
         */
        void ReceiveFrame();

        virtual void handleMessageWhenUp(cMessage *msg) override;

        virtual void handleStartOperation(LifecycleOperation *operation) override;
        virtual void handleStopOperation(LifecycleOperation *operation) override;
        virtual void handleCrashOperation(LifecycleOperation *operation) override;

};

#endif /* CLIENT_DASHPLAYER_H_ */
