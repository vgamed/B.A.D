#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include <cstring>
#include "StateMachine.h"
#include "Character.h"

USING_NS_CC;
using namespace cocostudio;

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
					 Armature* arm )
{
	m_info = info;
	setBasicCharState( basicState );
	m_pArmature = arm;
	m_pStateMachine = new StateMachine<Character>(this);

	if( m_pArmature )
		m_pArmature->retain();

	m_pTarget = nullptr;
	m_pListener = nullptr;

	Director::getInstance()->getEventDispatcher()->addCustomEventListener( "DummyGameEvent", 
		[&](EventCustom* event){ this->onCustomEvent(event); } );
}
