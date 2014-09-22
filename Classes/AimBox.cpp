#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "StateMachine.h"
#include "Character.h"
#include "AimBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

AimBox::AimBox(void)
{
	m_pCharBound = nullptr;
	m_showUp = false;
}


AimBox::~AimBox(void)
{
}

AimBox* AimBox::create( Vec2 pos, bool show,
					   cocos2d::Color4F color,
					   float width,	float height )
{
	AimBox* box = new AimBox();
	if( box && box->init( pos, show, width, height, color ) )
		box->autorelease();
	else
		CC_SAFE_DELETE( box );

	return box;
}

bool AimBox::init( cocos2d::Vec2& pos, bool show, 
	float width, float height, cocos2d::Color4F& color )
{
	if( !DrawNode::init() )
		return false;

	setBoxPosition( pos );
	setBoxWidth( width );
	setBoxHeight( height );
	setBoxColor( color );

	showUp( show );

	return true;
}

void AimBox::showUp( bool show )
{
	if( show )
		this->drawBox( m_pos, m_color );
	else
		this->drawBox( Vec2(-100.0f,-100.0f), Color4F(0.0f,0.0f,0.0f,0.0f) );
}

void AimBox::drawBox( Vec2 pos, Color4F color )
{
	Vec2 verts[4];
	verts[0] = Vec2( pos.x-m_width/2, pos.y );
	verts[1] = Vec2( pos.x-m_width/2, pos.y+m_height );
	verts[2] = Vec2( pos.x+m_width/2, pos.y+m_height );
	verts[3] = Vec2( pos.x+m_width/2, pos.y );

	this->clear();
	//this->drawPolygon( verts, 4, Color4F(0.0f,0.0f,0.0f,0.0f), 2.0f, color );
	this->drawSegment( verts[0], Vec2(verts[0].x+15.0f,verts[0].y), 2.0f, color );
	this->drawSegment( verts[0], Vec2(verts[0].x,verts[0].y+15.0f), 2.0f, color );

	this->drawSegment( verts[1], Vec2(verts[1].x+15.0f,verts[1].y), 2.0f, color );
	this->drawSegment( verts[1], Vec2(verts[1].x,verts[1].y-15.0f), 2.0f, color );

	this->drawSegment( verts[2], Vec2(verts[2].x-15.0f,verts[2].y), 2.0f, color );
	this->drawSegment( verts[2], Vec2(verts[2].x,verts[2].y-15.0f), 2.0f, color );

	this->drawSegment( verts[3], Vec2(verts[3].x-15.0f,verts[3].y), 2.0f, color );
	this->drawSegment( verts[3], Vec2(verts[3].x,verts[3].y+15.0f), 2.0f, color );
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
