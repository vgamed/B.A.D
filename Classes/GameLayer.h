#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"

class Character;

class GameLayer :
	public cocos2d::Layer
{
public:

	GameLayer(void)
	{
		m_phyWorld = nullptr;
	}

	~GameLayer(void)
	{
	}

    virtual bool init(void)
	{
		return Layer::init();
	}

	virtual Character* selectNodeByPosition( cocos2d::Point& pos, bool isForPlayer ) = 0;

	CC_SYNTHESIZE( cocos2d::PhysicsWorld*, m_phyWorld, PhyWorld );
};

#endif //__GAME_LAYER_H__