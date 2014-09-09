#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "GameLayer.h"

class AimBox;
class Character;
class GameMode;
class GameLayer;

class FightScene :
	public GameLayer
{
public:
	FightScene(void);
	~FightScene(void);

    static cocos2d::Scene* createScene();

    bool init();
	void update(float dt);
    
	GameMode* getGameMode(void) { return m_pGameMode; }

	void onEnterTransitionDidFinish(void);

    void menuPauseCallback(cocos2d::Ref* pSender);
    void menuQuitGameCallback(cocos2d::Ref* pSender);

	Character* selectNodeByPosition( cocos2d::Point& pos, bool isForPlayer );
    
    CREATE_FUNC(FightScene);
	
	CC_SYNTHESIZE( AimBox*, m_aimBox, AimBox );

private:
	GameMode* m_pGameMode;
	cocos2d::Layer* m_pPauseLayer;
};

#endif //__FIGHT_SCENE_H__

