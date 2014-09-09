#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "TargetBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

TargetBox::TargetBox(void)
{
}


TargetBox::~TargetBox(void)
{
}

TargetBox* TargetBox::create( Vec2& pos, bool show )
{
	TargetBox* box = new TargetBox();
	if( box && box->init( pos, show ) )
		box->autorelease();
	else
		CC_SAFE_DELETE( box );

	return box;
}

bool TargetBox::init( Vec2& pos, bool show )
{
	if( !DrawNode::init() )
		return false;

	setBoxPosition( pos );
	showUp( show );

	return true;
}

void TargetBox::showUp( bool show )
{
	if( show )
		this->drawBox( m_pos, Color4F(1.0f,0.0f,0.0f,0.8f) );
	else
		this->drawBox( Vec2(-100.0f,-100.0f), Color4F(0.0f,0.0f,0.0f,0.0f) );
}

void TargetBox::drawBox( Vec2& pos, Color4F& color )
{
	Vec2 verts[4];
	verts[0] = Vec2( pos.x-40.0f, pos.y-40.0f );
	verts[1] = Vec2( pos.x-40.0f, pos.y+40.0f );
	verts[2] = Vec2( pos.x+40.0f, pos.y+40.0f );
	verts[3] = Vec2( pos.x+40.0f, pos.y-40.0f );

	this->drawPolygon( verts, 4, Color4F(0.0f,0.0f,0.0f,0.0f), 5.0f, color );
}
