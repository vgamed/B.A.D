#ifndef __BASE_GAME_MODE_H__
#define __BASE_GAME_MODE_H__

class GameMode
{
public:
	GameMode(void) {}
	virtual ~GameMode(void) {}

	virtual bool initGame(cocos2d::Layer * layer) = 0;
	virtual bool startGame(void) = 0;
	virtual void updateGame(float dt) = 0;
	virtual void endGame(void)=0;
	virtual void resetGame(void) = 0;

	bool isGameStarted(void) { return m_bGameStarted; }
	void setGameStarted( bool start ) { m_bGameStarted = start; }

private:
	bool m_bGameStarted;
};

#endif //__BASE_GAME_MODE_H__