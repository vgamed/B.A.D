#ifndef __GAME_MONSTER_H__
#define __GAME_MONSTER_H__

class Character;
template <class T> class State;

class Monster :
	public Character
{
public:
	Monster(void);
	~Monster(void);

	void init( const CharInfo& info, const CharState& basicState,
		cocostudio::Armature* arm );

	void deploy( cocos2d::Layer* layer, const cocos2d::Vec2& pos );
	void update( float dt );
	void onCustomEvent( cocos2d::EventCustom* event );
	void onAnimationEvent( cocostudio::Armature* arm, 
		cocostudio::MovementEventType type, const std::string& movementID );

	friend class StateMonsterGlobal;

	State<Character>*	STATE_GLOBAL;
	State<Character>*	STATE_IDLE;
	State<Character>*	STATE_DEAD;

private:
	void doDamage( float phy_atk, float mgc_atk );
	void calcActualState(void);
};

// Monster global state
class StateMonsterGlobal : public State<Character>
{
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );
};

// Monster idle state
class StateMonsterIdle : public State<Character>
{
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );
};

// Monster Dead State
class StateMonsterDead : public State<Character>
{
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );
};

#endif //__GAME_MONSTER_H__