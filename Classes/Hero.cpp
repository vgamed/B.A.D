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
#include "Hero.h"
#include "EventDummyGame.h"

USING_NS_CC;
using namespace cocostudio;

Hero::Hero(void)
{
	STATE_GLOBAL	= nullptr;
	STATE_IDLE		= nullptr;
	STATE_MOVE		= nullptr;
	STATE_ATTACK	= nullptr;
}

Hero::~Hero(void)
{
}

void Hero::init( const CharInfo& info, const CharState& basicState,
	Armature* arm )
{
	Character::init( info, basicState, arm );

	STATE_GLOBAL	= new StateHeroGlobal();
	STATE_IDLE		= new StateHeroIdle();
	STATE_MOVE		= new StateHeroMove();
	STATE_ATTACK	= new StateHeroAttack();

	m_vecStates.push_back( STATE_MOVE );
	m_vecStates.push_back( STATE_GLOBAL );
	m_vecStates.push_back( STATE_IDLE );
	m_vecStates.push_back( STATE_ATTACK );
}

void Hero::deploy( cocos2d::Layer* layer, const cocos2d::Vec2& pos )
{
	//restore actual state
	setBasicCharState( m_basicState );

	//clear the target
	setTarget( nullptr );
	//reset state machine
	m_pStateMachine->setGlobalState( STATE_GLOBAL );
	m_pStateMachine->setCurrentState( STATE_IDLE );

	//deploy position
	if( layer && m_pArmature )
	{
		//m_pArmature->setPosition( pos );
		m_pArmature->setAnchorPoint( Vec2(0.5f, 0.0f) );

		if( !layer->getChildByTag( m_info.id ) )
			layer->addChild( m_pArmature, m_info.zorder, m_info.id );

		auto action = Place::create( pos );
		m_pArmature->runAction( action );
	}
}

void Hero::update( float dt )
{
	m_pStateMachine->update( dt );
}

void Hero::onCustomEvent( EventCustom* event )
{
}

void Hero::onAnimationEvent( Armature* arm, MovementEventType type, const char* movementID )
{
	if( type == COMPLETE )
	{
		EventDummyGame::DGEInfo info;
		info.type = EventDummyGame::DGE_ATTACK_SINGLE;
		info.sender = this;
		info.receiver = this->getTarget();
		EventDummyGame* e = new EventDummyGame( info );
		Director::getInstance()->getEventDispatcher()->dispatchEvent( e );
		CC_SAFE_DELETE( e );
	}
}

void Hero::doDamage( float phy_atk, float mgc_atk )
{
}

bool Hero::findTarget( Team* enemies )
{
	if( enemies == nullptr )
		return false;

	// quickly locate the target, temporarily
	if( !enemies->tm.m.bm || enemies->tm.m.bm->isDead() )
		return false;

	m_pTarget = enemies->tm.m.bm;
	return true;
}

void Hero::calcActualState(void)
{
	m_actualState = m_basicState;
}

////

// hero global state
void StateHeroGlobal::enter( Character* c )
{
}

void StateHeroGlobal::exec( Character* c, float dt )
{
	Hero* hero = dynamic_cast<Hero*>(c);
	if( hero )
	{
		CharState state = hero->getActualCharState();
		if( state.health > 0.0f )
		{
			// character states update
			state.health = MIN( state.maxHealth, state.health+state.healthRecover*dt );
			state.energy = MIN( state.maxEnergy, state.energy+state.energyRecover*dt );
			hero->setActualCharState( state );

			// check the target
			if( hero->getTarget() && hero->getTarget()->isDead() )
			{
				hero->setTarget( nullptr );
				hero->getStateMachine()->changeState( hero->STATE_IDLE );
			}
		}
		else
		{
			hero->getStateMachine()->changeState( hero->STATE_DEAD );
		}
	}
}

void StateHeroGlobal::exit( Character* c ) 
{
}
////

// hero idle state
void StateHeroIdle::enter( Character* c )
{
	auto scene = ((AppDelegate *)Application::getInstance())->getSceneFight();
	FightScene* fight = dynamic_cast<FightScene*>(scene->getChildByName("Fight"));
	if( !fight ) 
		return;

	DummyGameMode* mode = dynamic_cast<DummyGameMode*>(fight->getGameMode());
	m_teamPlayer = &mode->getPlayerTeam();
	m_teamEnemy = &mode->getEnemyTeam();

	Armature* arm = c->getArmature();
	if( arm )
		arm->getAnimation()->play( "loading" );
}

void StateHeroIdle::exec( Character* c, float dt )
{
	Hero* hero = dynamic_cast<Hero*>(c);
	if( hero && (hero->getTarget()==nullptr) && hero->findTarget( m_teamEnemy ) )
		hero->getStateMachine()->changeState( hero->STATE_MOVE );
}

void StateHeroIdle::exit( Character* c ) 
{
}
////

// hero move state
void StateHeroMove::enter( Character* c ) 
{
	if( !c )
		return;

	// play the "run" animation
	auto arm = c->getArmature();
	if( arm )
		arm->getAnimation()->play( "run" );
}

void StateHeroMove::exec( Character* c, float dt ) 
{
	if( !c )
		return;

	auto arm = c->getArmature();
	auto arm_t = c->getTarget()->getArmature();

	// calculate the position moves to
	Vec2 move_to;
	move_to.y = arm_t->getPositionY();
	move_to.x = arm_t->getBoundingBox().getMinX();
	auto cur = arm->getPosition();

	// detect if the target is reached
	float dist = cur.getDistance( move_to );
	if( dist < c->getActualCharState().attackDistance )
	{
		Hero* hero = dynamic_cast<Hero*>(c);
		if( hero )
		{
			hero->getStateMachine()->changeState( hero->STATE_ATTACK );
			return;
		}
	}
	
	// calculate the move increase of this update
	Vec2 inc = move_to - cur;
	inc.normalize();
	inc *= c->getActualCharState().speed * dt;
	arm->setPosition( cur+inc );

	//// run MoveTo Action
	//float dist = cur.getDistance( move_to );
	//float duration = dist / c->getActualCharState().speed;
	//m_actionMove = MoveTo::create( duration, move_to);
	//m_actionMove->retain();
	//arm->runAction( m_actionMove );
	//if( m_actionMove && m_actionMove->isDone() )
	//{
	//	Hero* hero = dynamic_cast<Hero*>(c);
	//	if( hero )
	//		hero->getStateMachine()->changeState( hero->STATE_ATTACK );
	//}
}

void StateHeroMove::exit( Character* c ) 
{
	//if( !c )
	//	return;
	//auto arm = c->getArmature();
	//arm->stopAction( m_actionMove );
	//if( m_actionMove )
	//	m_actionMove->release();
}
////

// hero attack state
void StateHeroAttack::enter( Character* c ) 
{
	if( !c )
		return;

	m_timeToAttack = 0.0f;

	// the very first attack
	if( Armature* arm = c->getArmature() )
	{
		arm->getAnimation()->play( "attack" );
		//arm->getAnimation()->setFrameEventCallFunc();
		arm->getAnimation()->setMovementEventCallFunc( c, movementEvent_selector(Hero::onAnimationEvent) );
	}
}

void StateHeroAttack::exec( Character* c, float dt ) 
{
	m_timeToAttack += dt;
	if( m_timeToAttack > c->getActualCharState().attackInterval )
	{
		Armature* arm = c->getArmature();
		if( arm && !arm->getAnimation()->isPlaying() )
		{
			arm->getAnimation()->play( "attack" );
			
			m_timeToAttack = 0.0f;
		}
	}
}

void StateHeroAttack::exit( Character* c ) 
{
	if( Armature* arm = c->getArmature() )
		arm->getAnimation()->setMovementEventCallFunc( nullptr, nullptr );
}
////
