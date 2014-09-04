#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "AppDelegate.h"
#include "StateMachine.h"
#include "Character.h"
#include "Team.h"
#include "GameMode.h"
#include "DummyGameMode.h"
#include "FightScene.h"

#include "EventDummyGame.h"

USING_NS_CC;
using namespace cocostudio;

EventDummyGame::EventDummyGame( const DGEInfo& info )
	: EventCustom( "DummyGameEvent" )
{
	setEventInfo( info );
}


EventDummyGame::~EventDummyGame(void)
{
}
