#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "FightScene.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;

const Size AppDelegate::DESIGN_RESOLUTION = Size(960.0f, 640.0f);

AppDelegate::AppDelegate() {
	m_sceneHello = nullptr;
	m_sceneFight = nullptr;
}

AppDelegate::~AppDelegate() 
{
}

// header files for fix memory leaks
#include "cocostudio/WidgetReader/LayoutReader/LayoutReader.h"
#include "cocostudio/WidgetReader/LoadingBarReader/LoadingBarReader.h"
#include "cocostudio/WidgetReader/ImageViewReader/ImageViewReader.h"

void AppDelegate::cleanCocos()
{
	if( m_sceneHello )
		m_sceneHello->release();

	if( m_sceneFight )
		m_sceneFight->release();

	auto pLayoutReader = LayoutReader::getInstance();
	CC_SAFE_DELETE( pLayoutReader );
	auto pLoadingBarReader = LoadingBarReader::getInstance();
	CC_SAFE_DELETE( pLoadingBarReader );
	auto pImageViewReader = ImageViewReader::getInstance();
	CC_SAFE_DELETE( pImageViewReader );

	ArmatureDataManager::destroyInstance();
	SceneReader::destroyInstance();
	ActionManagerEx::destroyInstance();
	GUIReader::destroyInstance();
	ObjectFactory::destroyInstance();
}

void AppDelegate::createScenes()
{
    m_sceneHello = HelloWorld::createScene();
	m_sceneHello->retain();

	m_sceneFight = FightScene::createScene();
	m_sceneFight->retain();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

	// setting design resolution
	glview->setDesignResolutionSize(DESIGN_RESOLUTION.width, 
		DESIGN_RESOLUTION.height, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	CCLOG( "CCLOG>> Game launching!!!" );

	CCLOG( "CCLOG>> Game data loaded!!!" );

	createScenes();

    // run
    director->runWithScene(m_sceneHello);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
