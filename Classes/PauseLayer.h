#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

class Character;
class GameLayer;
class AimBox;
class TargetBox;

class PauseLayer :
	public cocos2d::Layer
{
public:
	PauseLayer(void);
	~PauseLayer(void);

    bool init(void);

	void setBoundLayer( GameLayer* layer )
	{
		m_pLayerBound = layer; 
		if( m_pLayerBound )
			m_pLayerBound->retain(); 
	}
	const GameLayer* getBoundLayer(void) { return m_pLayerBound; }

	void processTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );

	CREATE_FUNC( PauseLayer );

private:
	GameLayer* m_pLayerBound;
	Character* m_pAimer;
	Character* m_pTarget;
	bool m_isSelectingTarget;

	AimBox* m_aimBox;
	TargetBox* m_targetBox;
};

#endif //__PAUSE_LAYER_H__

