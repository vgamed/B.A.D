#ifndef __AIM_BOX_H__
#define __AIM_BOX_H__

class Character;

class AimBox :
	public cocos2d::DrawNode
{
public:
	AimBox(void);
	~AimBox(void);

	static AimBox* create( cocos2d::Vec2 pos, bool show,
		cocos2d::Color4F color = cocos2d::Color4F(1.0f,1.0f,1.0f,0.6f),
		float width = 80.0f,
		float height = 80.0f );

	bool init( cocos2d::Vec2& pos, bool show, 
		float width, float height, cocos2d::Color4F& color );

	void showUp( bool show );
	bool isShownUp( void ) { return m_showUp; }

	CC_SYNTHESIZE( float, m_width, BoxWidth );
	CC_SYNTHESIZE( float, m_height, BoxHeight );

	CC_SYNTHESIZE_PASS_BY_REF( cocos2d::Vec2, m_pos, BoxPosition );
	CC_SYNTHESIZE_PASS_BY_REF( cocos2d::Color4F, m_color, BoxColor );

	CC_SYNTHESIZE( Character*, m_pCharBound, CharBound );

private:
	void drawBox( cocos2d::Vec2 pos, cocos2d::Color4F color );
	
	bool m_showUp;
};

#endif //__AIM_BOX_H__

