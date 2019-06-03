/*
 * DashPlayer.cpp
 *
 *  Created on: 3 de jun de 2019
 *      Author: futebol
 */

#include "DashPlayer.h"

Define_Module(DashPlayer);

DashPlayer::DashPlayer() {
    // TODO Auto-generated constructor stub

}

DashPlayer::~DashPlayer() {
    // TODO Auto-generated destructor stub
}

void DashPlayer::initialize(int stage)
{
    ApplicationBase::initialize(stage);
}

void DashPlayer::handleTimer(cMessage *msg)
{
    switch (msg->getKind()) {
        case MPEG_PLAYER_PLAYING:
            break;
        case MPEG_PLAYER_PAUSED:
            break;
        case MPEG_PLAYER_DONE:
            return;
        case MPEG_PLAYER_NOT_STARTED:
            break;
    }
}

void DashPlayer::handleMessageWhenUp(cMessage *msg)
{
    // TODO
}

void DashPlayer::handleStartOperation(LifecycleOperation *operation)
{
    // TODO
}

void DashPlayer::handleStopOperation(LifecycleOperation *operation)
{
    // TODO
}

void DashPlayer::handleCrashOperation(LifecycleOperation *operation)
{
    // TODO
}
