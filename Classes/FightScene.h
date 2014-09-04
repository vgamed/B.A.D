#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

class GameMode;

class FightScene :
	public cocos2d::Layer
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
    
    CREATE_FUNC(FightScene);

private:
	GameMode* m_pGameMode;
};

#endif //__FIGHT_SCENE_H__

