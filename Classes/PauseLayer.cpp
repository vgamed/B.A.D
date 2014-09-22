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
#include "AimBox.h"
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
	putBackCharToLayerBound( m_aimBox );
	putBackCharToLayerBound( m_targetBox );

	if( m_pLayerBound )
		m_pLayerBound->release();
	if( m_aimBox )
		m_aimBox->release();
	if( m_targetBox )
		m_targetBox->release();
}

bool PauseLayer::init(void)
{
	if( Layer::init() )
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches( false );
		listener->onTouchBegan = [](Touch* touch, Event* event) -> bool { return true; };
		listener->onTouchMoved = [](Touch* touch, Event* event) {};
		listener->onTouchEnded = [&](Touch* touch, Event* event) { this->processTouchEnded( touch, event );	};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
		
		// create background
		Vec2 verts[4];
		verts[0] = Vec2( 0.0f, AppDelegate::DESIGN_RESOLUTION.height/8 );
		verts[1] = Vec2( 0.0f, AppDelegate::DESIGN_RESOLUTION.height*2/3 );
		verts[2] = Vec2( AppDelegate::DESIGN_RESOLUTION.width, AppDelegate::DESIGN_RESOLUTION.height*2/3 );
		verts[3] = Vec2( AppDelegate::DESIGN_RESOLUTION.width, AppDelegate::DESIGN_RESOLUTION.height/8 );
		auto bg = DrawNode::create();
		bg->drawPolygon( verts, 4, Color4F(0.1f,0.1f,0.1f,0.75f), 1, Color4F(1.0f,1.0f,1.0f,1.0f) );
		addChild( bg, 0 );

		// create TargetBox
		m_targetBox = AimBox::create( Vec2(0.0f,0.0f), false, Color4F(1.0f,0.0f,0.0f,1.0f) );
		m_targetBox->retain();
		addChild( m_targetBox, 100 );

		// create AimerBox
		m_aimBox = AimBox::create( Vec2(0.0f,0.0f), false, Color4F(0.0f,1.0f,0.0f,1.0f) );
		m_aimBox->retain();
		addChild( m_aimBox, 100 );

		return true;
	}
	return false;
}

void PauseLayer::bringCharFromLayerBound( AimBox* box, Character* ch )
{
	if( box && ch && m_pLayerBound )
	{
		if( box->getCharBound() )
			putBackCharToLayerBound( box );

		m_pLayerBound->removeChild( ch->getArmature(), false );
		box->addChild( ch->getArmature() );

		box->setCharBound( ch );
	}
}

void PauseLayer::putBackCharToLayerBound( AimBox* box )
{
	if( box && box->getCharBound() && m_pLayerBound )
	{
		box->removeAllChildren();
		auto arm = box->getCharBound()->getArmature();
		m_pLayerBound->addChild( arm, 0, box->getCharBound()->getCharInfo().id );

		FightScene* fight = dynamic_cast<FightScene*>( m_pLayerBound );
		if( fight ) 
		{
			DummyGameMode* mode = dynamic_cast<DummyGameMode*>(fight->getGameMode());
			arm->setLocalZOrder( mode->calcZOrder(arm->getPositionY()) );
		}

		box->setCharBound( nullptr );
	}
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
#if CC_USE_PHYSICS
		PhysicsBody* body = aimer->getArmature()->getPhysicsBody();
		if( body )
		{
			auto box = dynamic_cast<PhysicsShapeBox*>( body->getFirstShape() );
			m_aimBox->setBoxPosition( aimer->getArmature()->getPosition() );
			m_aimBox->setBoxWidth( box->getSize().width );
			float h = aimer->getArmature()->getBoundingBox().getMaxY()
					- aimer->getArmature()->getBoundingBox().getMinY();
			m_aimBox->setBoxHeight( h );

			bringCharFromLayerBound( m_aimBox, aimer );
		}
#elif
		auto rect = aimer->getArmature()->getBoundingBox();
		m_aimBox->setBoxPosition( Vec2(rect.getMidX(),rect.getMidY()) );
		m_aimBox->setBoxWidth( rect.getMaxX()-rect.getMinX() );
		m_aimBox->setBoxHeight( rect.getMaxY()-rect.getMinY() );
#endif
		m_aimBox->showUp( true );
		if( auto t = aimer->getTarget() )
		{
			// show the target box
#if CC_USE_PHYSICS
			PhysicsBody* body = t->getArmature()->getPhysicsBody();
			if( body )
			{
				auto box = dynamic_cast<PhysicsShapeBox*>( body->getFirstShape() );
				m_targetBox->setBoxPosition( t->getArmature()->getPosition() );
				m_targetBox->setBoxWidth( box->getSize().width );
				float h = t->getArmature()->getBoundingBox().getMaxY()
						- t->getArmature()->getBoundingBox().getMinY();
				m_targetBox->setBoxHeight( h );

				bringCharFromLayerBound( m_targetBox, t );
			}
#elif
			auto rect = t->getArmature()->getBoundingBox();
			m_targetBox->setBoxPosition( Vec2(rect.getMidX(),rect.getMidY()) );
			m_targetBox->setBoxWidth( rect.getMaxX()-rect.getMinX() );
			m_targetBox->setBoxHeight( rect.getMaxY()-rect.getMinY() );
#endif
			m_targetBox->showUp( true );
			// draw curve to the target box
			//...
		}
		else // hide the target box
			m_targetBox->showUp( false );
		m_pAimer = aimer;
		return;
	}

	auto target = m_pLayerBound->selectNodeByPosition( pos, false );
	if( target && m_pAimer )
	{
		// move targeting box to the characters selected
#if CC_USE_PHYSICS
		PhysicsBody* body = target->getArmature()->getPhysicsBody();
		if( body )
		{
			auto box = dynamic_cast<PhysicsShapeBox*>( body->getFirstShape() );
			m_targetBox->setBoxPosition( target->getArmature()->getPosition() );
			m_targetBox->setBoxWidth( box->getSize().width );
			float h = target->getArmature()->getBoundingBox().getMaxY()
					- target->getArmature()->getBoundingBox().getMinY();
			m_targetBox->setBoxHeight( h );

			bringCharFromLayerBound( m_targetBox, target );
		}
#elif
		auto rect = target->getArmature()->getBoundingBox();
		m_targetBox->setBoxPosition( Vec2(rect.getMidX(),rect.getMidY()) );
		m_targetBox->setBoxWidth( rect.getMaxX()-rect.getMinX() );
		m_targetBox->setBoxHeight( rect.getMaxY()-rect.getMinY() );
#endif
		m_targetBox->showUp( true );
		m_pAimer->setTarget( target );
		// draw targeting curve
		//...
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