#include "cocos2d.h"
#include "GameMode.h"
#include "Team.h"
#include "DummyGameMode.h"
#include "AppDelegate.h"
#include "FightScene.h"

USING_NS_CC;

FightScene::FightScene(void)
{
}


FightScene::~FightScene(void)
{
	if( m_pGameMode )
		CC_SAFE_DELETE( m_pGameMode );
}

Scene* FightScene::createScene()
{
    auto scene = Scene::create();
    auto layer = FightScene::create();

    scene->addChild( layer, -100, "Fight" );

    return scene;
}

bool FightScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto quitItem = MenuItemFont::create( "Quit", CC_CALLBACK_1(FightScene::menuQuitGameCallback, this) );
	quitItem->setPosition( Vec2(origin.x+visibleSize.width-quitItem->getContentSize().width,
		origin.y+quitItem->getContentSize().height/2) );

	auto pauseItem = MenuItemFont::create( "Pause", CC_CALLBACK_1(FightScene::menuPauseCallback, this) );
	pauseItem->setTag(1002);
	pauseItem->setPosition( Vec2(origin.x+visibleSize.width/2, origin.y+pauseItem->getContentSize().height/2) );

    // create menu, it's an autorelease object
    auto menu = Menu::create(quitItem, pauseItem, NULL);
    menu->setPosition( Vec2::ZERO );
    this->addChild( menu, 1, 100 ); //tag 100

	// init game mode
	do{
		m_pGameMode = new DummyGameMode();
		CC_BREAK_IF( !m_pGameMode );
		CC_BREAK_IF( !m_pGameMode->initGame(this) );
		
		return true;
	}while(0);

	CC_SAFE_DELETE( m_pGameMode );
	return false;
}

void FightScene::update( float dt )
{
	m_pGameMode->updateGame( dt );
}

void FightScene::onEnterTransitionDidFinish(void)
{
	Node::onEnterTransitionDidFinish();

	if( !m_pGameMode->isGameStarted() )
		m_pGameMode->startGame();

	scheduleUpdate();
}

void FightScene::menuQuitGameCallback(Ref* pSender)
{
	m_pGameMode->endGame();

	//Director::getInstance()->popScene();
	Director::getInstance()->replaceScene( 
		((AppDelegate *)Application::getInstance())->getSceneHello() );
}

void FightScene::menuPauseCallback(Ref* pSender)
{
	auto menu_item = dynamic_cast<MenuItemFont*>( getChildByTag(100)->getChildByTag(1002) );
	if( Director::getInstance()->isPaused() )
	{
		menu_item->setString( "Pause" );
		Director::getInstance()->resume();
	}
	else
	{
		menu_item->setString( "Resume" );
		Director::getInstance()->pause();
	}
}
