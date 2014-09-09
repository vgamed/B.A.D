#ifndef __AIM_BOX_H__
#define __AIM_BOX_H__

class AimBox :
	public cocos2d::DrawNode
{
public:
	AimBox(void);
	~AimBox(void);

	static AimBox* create( cocos2d::Vec2& pos, bool show );
	bool init( cocos2d::Vec2& pos, bool show );

	void setBoxPosition( cocos2d::Vec2& pos ) { m_pos = pos; }
	cocos2d::Vec2& getBoxPosition( void ) { return m_pos; }

	void showUp( bool show );
	bool isShownUp( void ) { return m_showUp; }

private:
	cocos2d::Vec2 m_pos;
	bool m_showUp;
};

#endif //__AIM_BOX_H__

