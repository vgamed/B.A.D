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
		m_team.ma[i] = nullptr;
	}
}


Team::~Team(void)
{
}

void Team::update( float dt )
{
	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		if( m_team.ma[i] )
			m_team.ma[i]->update( dt );
	}
}

void Team::clear(void)
{
	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		delete m_team.ma[i];
	}
}