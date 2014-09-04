#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "StateMachine.h"
#include "Character.h"
#include "Team.h"

USING_NS_CC;

Team::Team(void)
{
	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		tm.ma[i] = nullptr;
	}
}


Team::~Team(void)
{
}

void Team::update( float dt )
{
	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		if( tm.ma[i] )
			tm.ma[i]->update( dt );
	}
}

void Team::deploy( Layer* layer )
{
}

void Team::clear(void)
{
	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		delete tm.ma[i];
	}
}