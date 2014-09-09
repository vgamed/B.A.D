#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "AimBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

AimBox::AimBox(void)
{
}


AimBox::~AimBox(void)
{
}

AimBox* AimBox::create( Vec2& pos, bool show )
{
	AimBox* box = new AimBox();
	if( box && box->init( pos, show ) )
		box->autorelease();
	else
		CC_SAFE_DELETE( box );

	return box;
}

bool AimBox::init( Vec2& pos, bool show )
{
	if( !DrawNode::init() )
		return false;

	setBoxPosition( pos );
	showUp( show );

	return true;
}

void AimBox::showUp( bool show )
{
	if( show )
		this->drawDot( m_pos, 40.0f, Color4F(1.0f,1.0f,0.0f,0.8f) );
	else
		this->drawDot( Vec2(-100.0f,-100.0f), 40.0f, Color4F(0.0f,0.0f,0.0f,0.0f) );
}

//void AimBox::draw( Renderer* renderer, const Mat4& transform, bool transformUpdated )
//{
//	if( m_showUp )
//	{
//		glLineWidth( 5 );
//		float radius = 80.0f;
//		DrawPrimitives::drawCircle( m_pos, radius, 360, 20, true );
//		glLineWidth( 1 );
//	}
//}