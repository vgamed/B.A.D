#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include <cstring>
#include "StateMachine.h"
#include "Character.h"
#include "GameMode.h"
#include "Team.h"
#include "DummyGameMode.h"
#include "AppDelegate.h"
#include "GameLayer.h"
#include "PauseLayer.h"
#include "AimBox.h"
#include "FightScene.h"

USING_NS_CC;
using namespace cocostudio;

FightScene::FightScene(void)
{
	m_pGameMode = nullptr;
	m_pPauseLayer = nullptr;
}


FightScene::~FightScene(void)
{
	if( m_pGameMode )
		CC_SAFE_DELETE( m_pGameMode );
}

Scene* FightScene::createScene()
{
#if CC_USE_PHYSICS
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#elif
	auto scene = Scene::create();
#endif
    
	// add physics boundary
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto edgeNode = Node::create();

#if CC_USE_PHYSICS
	auto boundary = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
	edgeNode->setPhysicsBody( boundary );
#endif

	edgeNode->setPosition( Point(visibleSize.width/2,visibleSize.height/2) );
	scene->addChild( edgeNode, -100, "PhyBoundary" );

	// create the layer
	auto game_layer = FightScene::create();

#if CC_USE_PHYSICS
	game_layer->setPhyWorld( scene->getPhysicsWorld() );
#endif

	// add FightScene layer
	scene->addChild( game_layer, -100, "Fight" );

    return scene;
}

bool FightScene::init()
{
    if ( !GameLayer::init() )
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
	{
		m_pGameMode->startGame();
	}

	getPhyWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	this->getScene()->scheduleUpdate();
	this->scheduleUpdate();
}

void FightScene::menuQuitGameCallback(Ref* pSender)
{
	m_pGameMode->endGame();

	getPhyWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

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
		this->getParent()->removeChildByName( "Pause", true );
	}
	else
	{
		menu_item->setString( "Resume" );
		Director::getInstance()->pause();

		auto pause_layer = PauseLayer::create();
		pause_layer->setBoundLayer( this );
		this->getParent()->addChild( pause_layer, -90, "Pause");
	}
}

Character* FightScene::selectNodeByPosition( Point& pos, bool isForPlayer )
{
	auto gamemode = dynamic_cast<DummyGameMode*>( m_pGameMode );
	Team& team = isForPlayer ? gamemode->getPlayerTeam() : gamemode->getEnemyTeam();

	for( int i=0; i<NUM_TEAM_MEMBER; i++ )
	{
		Character* c = team.tm.ma[i];
		if( c == nullptr )
			continue;
		
		Armature* arm = team.tm.ma[i]->getArmature();
		if( arm == nullptr )
			continue;
		
		Rect rc = arm->getBoundingBox();
		if( rc.containsPoint( pos ) )
			return c;
	}

	return nullptr;
}