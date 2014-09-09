#ifndef __TARGET_BOX_H__
#define __TARGET_BOX_H__

class TargetBox :
	public cocos2d::DrawNode
{
public:
	TargetBox(void);
	~TargetBox(void);

	static TargetBox* create( cocos2d::Vec2& pos, bool show );
	bool init( cocos2d::Vec2& pos, bool show );

	void setBoxPosition( cocos2d::Vec2& pos ) { m_pos = pos; }
	cocos2d::Vec2& getBoxPosition( void ) { return m_pos; }

	void showUp( bool show );
	bool isShownUp( void ) { return m_showUp; }

private:
	void drawBox( cocos2d::Vec2& pos, cocos2d::Color4F& color );
	cocos2d::Vec2 m_pos;
	bool m_showUp;
};

#endif //__TARGET_BOX_H__

