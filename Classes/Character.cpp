#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include <cstring>
#include "StateMachine.h"
#include "Character.h"

USING_NS_CC;
using namespace cocostudio;

const Size Character::PHYSICS_BOX_SIZE = Size(80.0f, 50.0f);
const float Character::SCALE_X = 0.5f;
const float Character::SCALE_Y = 0.5f;

Character::Character(void)
{
	m_info.id = -1; //invalid id
	m_info.name = "";
	m_info.desc = "";
	m_info.level = 0;
	m_info.star = 0;
	m_info.color = 0;

	memset( &m_basicState, 0, sizeof(CharState) );
	memset( &m_actualState, 0, sizeof(CharState) );

	m_pStateMachine = nullptr;
	m_pArmature = nullptr;
	m_phyBody = nullptr;
	m_pTarget = nullptr;
}

Character::~Character(void)
{
	// reset the state machine
	m_pStateMachine->changeGlobalState( nullptr );

	std::vector<State<Character>*>::iterator it;
	for( it=m_vecStates.begin(); it!=m_vecStates.end(); ++it )
		delete (*it);

	delete m_pStateMachine;

	if( m_pArmature )
		m_pArmature->release();
}

void Character::init( const CharInfo& info, const CharState& basicState, 
					 Armature* arm, PhysicsBody* body )
{
	m_info = info;
	m_basicState = basicState;
	m_pArmature = arm;
	if( m_pArmature )
		m_pArmature->retain();

#if CC_USE_PHYSICS
	m_phyBody = body;
	if( body )
	{
		body->setGravityEnable( false );
		body->setPositionOffset( Vec2(0.0f,PHYSICS_BOX_SIZE.height*SCALE_X/2) );
		m_pArmature->setPhysicsBody( body );
	}
#endif

	m_pStateMachine = new StateMachine<Character>(this);

	Director::getInstance()->getEventDispatcher()->addCustomEventListener( "DummyGameEvent", 
		[&](EventCustom* event){ this->onCustomEvent(event); } );

	reset();
}

void Character::reset( void )
{
	// restore actualState to basicState
	setBasicCharState( m_basicState );

	// restore the X axis flipping by setting scaleX
	m_pArmature->setScaleX( m_actualState.facingTo.x * m_actualState.front.x * SCALE_X );
	m_pArmature->setScaleY( SCALE_Y );
	m_pArmature->setAnchorPoint( Vec2(0.5f, 0.0f) );

	// clear current target
	m_pTarget = nullptr;
}

bool Character::findDefaultTarget( Character** enemies, int number )
{
	if( enemies == nullptr )
		return false;

	auto pos = getArmature()->getPosition();
	float dist = FLT_MAX;
	m_pTarget = nullptr;

	// find the target by minimum distance
	for( int i=0; i<number; ++i )
	{
		auto c = enemies[i];
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

Vec2 Character::calcMoveTo( void )
{
	if( !m_pTarget )
		return Vec2( -1.0f, -1.0f );

	Vec2 m1 = m_pTarget->getArmature()->getPosition();
	m1.x += m_actualState.attackDistance;
	float dist1 = getArmature()->getPosition().getDistance( m1 );

	Vec2 m2 = m_pTarget->getArmature()->getPosition();
	m2.x -= m_actualState.attackDistance;
	float dist2 = getArmature()->getPosition().getDistance( m2 );

	return ( dist1 < dist2 ? m1 : m2 );
}

