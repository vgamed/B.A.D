#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "StateMachine.h"
#include "Character.h"
#include "Monster.h"
#include "EventDummyGame.h"

USING_NS_CC;
using namespace cocostudio;

// Monster class itself
Monster::Monster(void)
{
	STATE_GLOBAL	= nullptr;
	STATE_DEAD		= nullptr;
}

Monster::~Monster(void)
{
}

void Monster::init( const CharInfo& info, const CharState& basicState,
	cocostudio::Armature* arm )
{
	Character::init( info, basicState, arm );

	STATE_GLOBAL	= new StateMonsterGlobal();
	STATE_IDLE		= new StateMonsterIdle();
	STATE_DEAD		= new StateMonsterDead();

	m_vecStates.push_back( STATE_GLOBAL );
	m_vecStates.push_back( STATE_IDLE );
	m_vecStates.push_back( STATE_DEAD );
}

void Monster::deploy( cocos2d::Layer* layer, const cocos2d::Vec2& pos )
{
	//restore actual state
	setBasicCharState( m_basicState );

	//reset state machine
	m_pStateMachine->setGlobalState( STATE_GLOBAL );
	m_pStateMachine->setCurrentState( STATE_IDLE );

	//deploy position
	if( layer && m_pArmature )
	{
		m_pArmature->setPosition( pos );
		m_pArmature->setAnchorPoint( Vec2(0.5f, 0.0f) );

		if( !layer->getChildByTag( m_info.id ) )
			layer->addChild( m_pArmature, m_info.zorder, m_info.id );
	}
}

void Monster::update( float dt )
{
	m_pStateMachine->update( dt );
}

void Monster::onCustomEvent( EventCustom* event )
{
	auto e = dynamic_cast<EventDummyGame*>( event );
	if( e->receiver() != this )
		return;

	switch( e->type() )
	{
	case EventDummyGame::DGE_ATTACK_SINGLE:
		doDamage( e->sender()->getActualCharState().phyAttack,
			e->sender()->getActualCharState().mgcAttack );
		break;
	case EventDummyGame::DGE_ATTACK_ALL:
		break;
	case EventDummyGame::DGE_ATTACK_SPECIAL:
		break;
	case EventDummyGame::DGE_DISABLE_SKILL:
		break;
	case EventDummyGame::DGE_DIZZY_SKILL:
		break;
	}
}

void Monster::onAnimationEvent( Armature* arm, MovementEventType type, const char* movementID )
{
}

void Monster::doDamage( float phy_atk, float mgc_atk )
{
	float damage = MAX(phy_atk*(1.0f-m_actualState.phyResist),0.0f) +
		MAX(mgc_atk*(1.0f-m_actualState.mgcResist),0.0f);

	m_actualState.health = MAX(m_actualState.health-damage, 0.0f);
}

void Monster::calcActualState(void)
{
	m_actualState = m_basicState;
}
////

// Monster Global State
void StateMonsterGlobal::enter( Character* c ) 
{
}

void StateMonsterGlobal::exec( Character* c, float dt ) 
{
	auto monster = dynamic_cast<Monster*>(c);
	if( monster )
	{
		CharState state = monster->getActualCharState();
		if( state.health > 0.0f )
		{
			// character states update
			state.health = MIN( state.maxHealth, state.health+state.healthRecover*dt );
			state.energy = MIN( state.maxEnergy, state.energy+state.energyRecover*dt );
			monster->setActualCharState( state );

			// check the target
			if( monster->getTarget() && monster->getTarget()->isDead() )
			{
				monster->setTarget( nullptr );
				monster->getStateMachine()->changeState( monster->STATE_IDLE );
			}
		}
		else if( !monster->getStateMachine()->isInState(monster->STATE_DEAD) )
		{
			monster->getStateMachine()->changeState( monster->STATE_DEAD );
		}
	}
}

void StateMonsterGlobal::exit( Character* c ) 
{
}
////

// Monster Idle State
void StateMonsterIdle::enter( Character* c )
{
	cocostudio::Armature* arm = c->getArmature();
	if( arm )
		arm->getAnimation()->play( "loading" );
}

void StateMonsterIdle::exec( Character* c, float dt ) 
{
}

void StateMonsterIdle::exit( Character* c ) 
{
}
////

// Monster Move State
////

// Monster Attack State
////

// Monster Damage State
////

// Monster Dead State
void StateMonsterDead::enter( Character* c )
{
	cocostudio::Armature* arm = c->getArmature();
	if( arm )
		arm->getAnimation()->play( "death" );
}

void StateMonsterDead::exec( Character* c, float dt ) 
{
}

void StateMonsterDead::exit( Character* c ) 
{
}
////