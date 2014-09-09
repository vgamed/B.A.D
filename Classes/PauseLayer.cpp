#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "StateMachine.h"
#include "Character.h"
#include "AimBox.h"
#include "TargetBox.h"
#include "GameLayer.h"
#include "PauseLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

PauseLayer::PauseLayer(void)
{
	m_pLayerBound = nullptr;
	m_pAimer = nullptr;
	m_pTarget = nullptr;
	m_isSelectingTarget = false;
	m_aimBox = nullptr;
	m_targetBox = nullptr;
}


PauseLayer::~PauseLayer(void)
{
	if( m_pLayerBound )
		m_pLayerBound->release();
	if( m_aimBox )
		m_aimBox->release();
	if( m_targetBox )
		m_targetBox->release();
}

bool PauseLayer::init(void)
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( false );
	listener->onTouchBegan = [](Touch* touch, Event* event) -> bool { return true; };
	listener->onTouchMoved = [](Touch* touch, Event* event) {};
	listener->onTouchEnded = [&](Touch* touch, Event* event) { this->processTouchEnded( touch, event );	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
	// create background

	// create TargetBox
	m_targetBox = TargetBox::create( Vec2(0.0f,0.0f), false );
	m_targetBox->retain();
	addChild( m_targetBox, 100 );

	// create AimerBox
	m_aimBox = AimBox::create( Vec2(0.0f,0.0f), false );
	m_aimBox->retain();
	addChild( m_aimBox, 100 );

	return true;
}

void PauseLayer::processTouchEnded( Touch* touch, Event* event )
{
	// select aimer or target
	auto touchPos = touch->getLocationInView();
	auto pos = Director::getInstance()->convertToUI( touchPos );

	auto aimer = m_pLayerBound->selectNodeByPosition( pos, true );
	if( aimer )
	{
		// move aiming box to the characters selected
		m_aimBox->setBoxPosition( pos );
		m_aimBox->showUp( true );
		if( aimer->getTarget() )
		{
			// show the target box
			//...
			// draw curve to the target box
			//...
		}
		m_pAimer = aimer;
		m_isSelectingTarget = true;
		return;
	}

	auto target = m_pLayerBound->selectNodeByPosition( pos, false );
	if( target && m_isSelectingTarget )
	{
		// move targeting box to the characters selected
		m_targetBox->setBoxPosition( pos );
		m_targetBox->showUp( true );
		m_pAimer->setTarget( target );
		// draw targeting curve
		//...
		m_isSelectingTarget = false;
	}

	//do{
	//	if( m_isSelectingTarget )
	//	{
	//		m_pTarget = m_pLayerBound->selectNodeByPosition( pos, false );
	//		CC_BREAK_IF( !m_pTarget );
	//		// move targeting box to the characters selected
	//		m_targetBox->setBoxPosition( pos );
	//		m_targetBox->showUp( true );
	//		m_pAimer->setTarget( m_pTarget );
	//		// draw targeting curve
	//		m_isSelectingTarget = false;
	//	}
	//	else
	//	{
	//		m_pAimer = m_pLayerBound->selectNodeByPosition( pos, true );
	//		CC_BREAK_IF( !m_pAimer );
	//		// move aiming box to the characters selected
	//		m_aimBox->setBoxPosition( pos );
	//		m_aimBox->showUp( true );
	//		if( m_pAimer->getTarget() )
	//		{
	//			// show the target box
	//			// draw curve to the target box
	//		}
	//		m_isSelectingTarget = true;
	//	}
	//}while(0);
}