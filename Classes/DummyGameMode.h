#ifndef __DUMMY_GAME_MODE_H__
#define __DUMMY_GAME_MODE_H__

class GameMode;
class Team;

class DummyGameMode :
	public GameMode
{
public:
	// offset of battle field from design resolution
	static const cocos2d::Size BFLD_OFFSET;
	static const float BFLD_OFF_U;
	static const float BFLD_OFF_B;
	static const float BFLD_OFF_L;
	static const float BFLD_OFF_R;
	// offset of deployment slot from battle field
	static const cocos2d::Size SLOT_OFFSET;

	// left_bottom and right_top of the battle field
	static const cocos2d::Vec2 BF_LT_BTM;
	static const cocos2d::Vec2 BF_RT_TOP;

	// Forward/Middl/Back; Upper/Middle/Bottom
	static const cocos2d::Vec2 PLAYER_F_U;
	static const cocos2d::Vec2 PLAYER_F_M;
	static const cocos2d::Vec2 PLAYER_F_B;
	static const cocos2d::Vec2 PLAYER_M_U;
	static const cocos2d::Vec2 PLAYER_M_M;
	static const cocos2d::Vec2 PLAYER_M_B;
	static const cocos2d::Vec2 PLAYER_B_U;
	static const cocos2d::Vec2 PLAYER_B_M;
	static const cocos2d::Vec2 PLAYER_B_B;

	static const cocos2d::Vec2 MONSTER_F_U;
	static const cocos2d::Vec2 MONSTER_F_M;
	static const cocos2d::Vec2 MONSTER_F_B;
	static const cocos2d::Vec2 MONSTER_M_U;
	static const cocos2d::Vec2 MONSTER_M_M;
	static const cocos2d::Vec2 MONSTER_M_B;
	static const cocos2d::Vec2 MONSTER_B_U;
	static const cocos2d::Vec2 MONSTER_B_M;
	static const cocos2d::Vec2 MONSTER_B_B;

	DummyGameMode(void);
	~DummyGameMode(void);

	bool initGame(cocos2d::Layer * layer);
	bool startGame(void);
	void updateGame(float dt);
	void endGame(void);
	void resetGame(void);

	Team& getHeroTeam(void) { return m_teamHero; }
	Team& getMonsterTeam(void) { return m_teamMonster; }

	int calcZOrder( float y );

private:
	cocos2d::Layer* m_layer;
	Team m_teamHero;
	Team m_teamMonster;

};

#endif //__DUMMY_GAME_MODE_H__