#include "AppDelegate.h"
#include "GameLayer.h"
#include "Player.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}
 
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("KanClear");
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(800, 450, ResolutionPolicy::SHOW_ALL);
    // turn on display FPS
    director->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //auto scene = MainMenuLayer::createScene();
	auto scene = Scene::create();
	Player* A = new Player(new SendaiKai2Character());
	Player_AI* B = new Player_AI(new CruiserECharacter());
	B->HardGrade = 3;
	auto gameLayer = BattleLayer_2P::createWithPlayer(A,B);
	scene->addChild(gameLayer);
	gameLayer->setContentSize(director->getVisibleSize());
    // run
	director->runWithScene(scene);
	gameLayer->initLayout();
	gameLayer->gameStart();
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
