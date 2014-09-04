#ifndef __GAME_HERO_H__
#define __GAME_HERO_H__

template <class T> class State;
class Character;
class Team;

class Hero :
	public Character
{
public:
	Hero(void);
	~Hero(void);

	void init( const CharInfo& info, const CharState& basicState,
		cocostudio::Armature* arm );

	void deploy( cocos2d::Layer* layer, const cocos2d::Vec2& pos );
	void update( float dt );

	void onCustomEvent( cocos2d::EventCustom* event );
	void onAnimationEvent( cocostudio::Armature* arm, 
		cocostudio::MovementEventType type, const char* movementID );

	bool findTarget( Team* enemies );

	friend class StateHeroGlobalState;
	friend class StateHeroMove;
	friend class StateHeroAttack;

	State<Character>*	STATE_GLOBAL;
	State<Character>*	STATE_IDLE;
	State<Character>*	STATE_MOVE;
	State<Character>*	STATE_ATTACK;
	State<Character>*	STATE_DEAD;

private:
	void doDamage( float phy_atk, float mgc_atk );
	void calcActualState(void);
};

// global state of hero
class StateHeroGlobal : public State<Character>
{
public:
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );
};

// idle state of hero
class StateHeroIdle : public State<Character>
{
public:
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );

private:
	Team* m_teamPlayer;
	Team* m_teamEnemy;
};

// move state of hero
class StateHeroMove : public State<Character>
{
public:
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );

private:
	cocos2d::Action* m_actionMove;
};

// attack state of hero
class StateHeroAttack : public State<Character>
{
public:
	void enter( Character* c );
	void exec( Character* c, float dt );
	void exit( Character* c );

private:
	float m_timeToAttack;
};

// dead state of hero
class StateHeroDead : public State<Character>
{
public:
	void enter( Character* c ){}
	void exec( Character* c, float dt ){}
	void exit( Character* c ){}
};

#endif //__GAME_HERO_H__