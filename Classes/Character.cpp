#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include <cstring>
#include "StateMachine.h"
#include "Character.h"

USING_NS_CC;
using namespace cocostudio;

const Size Character::PHYSICS_BOX_SIZE = Size(60.0f, 120.0f);

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
	m_pListener = nullptr;
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
	setBasicCharState( basicState );
	m_pArmature = arm;

#if CC_USE_PHYSICS
	if( body )
		m_pArmature->setPhysicsBody( body );
	//m_phyBody = body;
#endif

	m_pStateMachine = new StateMachine<Character>(this);

	if( m_pArmature )
	{
		m_pArmature->retain();
		m_pArmature->setScale( 0.5f );
	}

	m_pTarget = nullptr;
	m_pListener = nullptr;

	Director::getInstance()->getEventDispatcher()->addCustomEventListener( "DummyGameEvent", 
		[&](EventCustom* event){ this->onCustomEvent(event); } );
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

