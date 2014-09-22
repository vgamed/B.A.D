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
	CREATE_FUNC( PauseLayer );

	void setBoundLayer( GameLayer* layer )
	{
		m_pLayerBound = layer; 
		if( m_pLayerBound )
			m_pLayerBound->retain(); 
	}
	const GameLayer* getBoundLayer(void) { return m_pLayerBound; }

	void processTouchEnded( cocos2d::Touch* touch, cocos2d::Event* event );

protected:
	PauseLayer(void);
	~PauseLayer(void);

    bool init(void);

private:
	void bringCharFromLayerBound( AimBox* box, Character* ch );
	void putBackCharToLayerBound( AimBox* box );

	GameLayer* m_pLayerBound;
	Character* m_pAimer;
	Character* m_pTarget;
	bool m_isSelectingTarget;

	AimBox* m_aimBox;
	AimBox* m_targetBox;
};

#endif //__PAUSE_LAYER_H__

