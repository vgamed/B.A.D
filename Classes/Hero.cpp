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
	Armature* arm, PhysicsBody* body )
{
#if CC_USE_PHYSICS
	Character::init( info, basicState, arm, PhysicsBody::createBox(PHYSICS_BOX_SIZE) );
#elif
	Character::init( info, basicState, arm, body );
#endif

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
	reset();

	//reset state machine
	m_pStateMachine->setGlobalState( STATE_GLOBAL );
	m_pStateMachine->setCurrentState( STATE_IDLE );

	//deploy position
	if( layer && m_pArmature )
	{
		if( !layer->getChildByTag( m_info.id ) )
		{
			layer->addChild( m_pArmature, 0, m_info.id );
		}
		m_pArmature->setPosition( pos );
		FightScene* fight = dynamic_cast<FightScene*>( getArmature()->getParent() );
		if( fight ) 
		{
			DummyGameMode* mode = dynamic_cast<DummyGameMode*>(fight->getGameMode());
			m_pArmature->setLocalZOrder( mode->calcZOrder(m_pArmature->getPositionY()) );
		}
	}
}

void Hero::update( float dt )
{
	m_pStateMachine->update( dt );

	// update z orders of all characters
	FightScene* fight = dynamic_cast<FightScene*>( getArmature()->getParent() );
	if( m_pArmature && fight ) 
	{
		DummyGameMode* mode = dynamic_cast<DummyGameMode*>(fight->getGameMode());
		m_pArmature->setLocalZOrder( mode->calcZOrder(m_pArmature->getPositionY()) );
	}
}

void Hero::onCustomEvent( EventCustom* event )
{
}

void Hero::onAnimationEvent( Armature* arm, MovementEventType type, 
							const std::string& movementID )
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

bool Hero::findDefaultTarget( Team* enemies )
{
	if( enemies == nullptr )
		return false;

	auto pos = getArmature()->getPosition();
	float dist = FLT_MAX;
	m_pTarget = nullptr;

	// find the target by minimum distance
	for( int i=0; i<NUM_TEAM_MEMBER; ++i )
	{
		auto c = enemies->tm.ma[i];
		if( !c || c->isDead() )
			continue;
		auto arm_e = c->getArmature();
		Vec2 pos_e = arm_e->getPosition();
		float dist_e = pos.distance( pos_e );
		if( dist_e < dist )
		{
			dist = dist_e;
			m_pTarget = c;
		}
	}

	if( !m_pTarget )
		return false;

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
	if( hero && (hero->getTarget()||hero->findDefaultTarget( m_teamEnemy )) )
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
	auto state = c->getActualCharState();

	// calculate the position moves to
	Vec2 move_to = c->calcMoveTo();
	auto cur = arm->getPosition();

	// decide if the character needs to turn around
	Vec2 mov_dir = move_to - cur;
	auto dot = mov_dir.dot( state.facingTo );
	if( dot < 0 )
	{
		//float angle = arm->getRotationSkewY() ? 0.0f : 180.0f;
		//arm->setRotationSkewY( angle );
		state.facingTo.x = -state.facingTo.x;
		c->setActualCharState( state );
		arm->setScaleX( state.facingTo.x * state.front.x * 0.5f );
	}

	// detect if the target can be reached
	float dist =cur.getDistance( move_to );
	if( dist < state.attackDistance )
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
	inc *= state.moveSpeed * dt;
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
		//arm->getAnimation()->setMovementEventCallFunc( c, movementEvent_selector(Hero::onAnimationEvent) );
		arm->getAnimation()->setMovementEventCallFunc( [=](Armature *armature, MovementEventType movementType, const std::string& movementID)
														{
															c->onAnimationEvent( armature, movementType, movementID ); 
														} );
	}
}

void StateHeroAttack::exec( Character* c, float dt ) 
{
	auto arm = c->getArmature();
	auto arm_t = c->getTarget()->getArmature();

	// calculate the distance to the target
	Vec2 move_to = c->calcMoveTo();
	auto cur = arm->getPosition();
	float dist = cur.getDistance( move_to );

	// detect if the target is out of reach
	if( dist > c->getActualCharState().attackDistance )
	{
		Hero* hero = dynamic_cast<Hero*>(c);
		if( hero && hero->getTarget() )
			hero->getStateMachine()->changeState( hero->STATE_MOVE );
	}
	else
	{
		// play the attack animation based on the attacking interval
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
}

void StateHeroAttack::exit( Character* c ) 
{
	if( Armature* arm = c->getArmature() )
		arm->getAnimation()->setMovementEventCallFunc( nullptr );
}
////
