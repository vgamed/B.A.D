#include "cocos2d.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CCLOG("CCLOG visibleSize: width=%f, height=%f", visibleSize.width, visibleSize.height); 
	CCLOG("CCLOG origin: x=%f, y=%f", origin.x, origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));


	auto startItem = MenuItemFont::create( "Start Game", CC_CALLBACK_1(HelloWorld::menuStartGameCallback, this) );
	startItem->setPosition( Vec2(origin.x+visibleSize.width/2, origin.y+startItem->getContentSize().height/2) );

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // create a sprite with cocos logo
    auto sprite = Sprite::create("cocos_logo.png");
	// create a label with the game title
    auto label = LabelTTF::create("B. A. D. GAME", "Arial", 36);

    // position the cocos logo
	sprite->setAnchorPoint(Vec2(0.0f, 1.0f));
	sprite->setPosition(Vec2(origin.x+10, origin.y+visibleSize.height));
	sprite->setScale(0.5f);
    // position the game title label on the center of the screen
	label->setAnchorPoint(Vec2(0.5f, 0.5f));
	label->setPosition(Vec2(origin.x+visibleSize.width/2,
		origin.y+visibleSize.height/2));

    this->addChild(label, 1);
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

	((AppDelegate*)Application::getInstance())->cleanCocos();

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuStartGameCallback(Ref* pSender)
{
	auto app = (AppDelegate *)Application::getInstance();
	auto fight = app->getSceneFight();
	if( fight )
	{
		auto director = Director::getInstance();
		//director->pushScene( fight );
		director->replaceScene( fight );
	}
}
