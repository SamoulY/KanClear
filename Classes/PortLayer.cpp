#include "PortLayer.h"
#include "Buttons.h"
#include "CommanderInfoBar.h"
#include "Commander.h"
#include "GameLayer.h"
#include "Player.h"
#include "Ship.h"
#include "Skills.h";

using namespace cocos2d;

MainPortButton MainPortLayer::m_buttons[] = { 
	&MainPortLayer::m_attackbutton,
	&MainPortLayer::m_factorybutton,
	&MainPortLayer::m_modifybutton,
	&MainPortLayer::m_organizebutton,
	&MainPortLayer::m_missionbutton,
	&MainPortLayer::m_activitybutton,
	&MainPortLayer::m_pokedexbutton,
	&MainPortLayer::m_settingbutton
};

class Clock :public cocos2d::Sprite
{
public:
	static Clock* create(int fontsize = 33,const char* font = "times");
protected:
	Clock(int fontsize,const char* font) :m_font(font), m_fontsize(fontsize<6?6:fontsize){};
	bool init() override;
	int m_fontsize;
	time_t m_timenow;
	std::string m_font;
	cocos2d::Label* m_time;
	cocos2d::Label* m_date;
	void update(float dt) override;
};
Clock* Clock::create(int fontsize,const char* font)
{
	Clock* gl = new Clock(fontsize, font);
	if (gl != 0 && gl->init())
	{
		gl->autorelease();
	}
	else
	{
		delete(gl);
		gl = 0;
	}
	return gl;
}
bool Clock::init()
{
	if (!Sprite::init())
		return false;
	addChild(m_time = Label::createWithTTF(TTFConfig(String::createWithFormat("Resources/fonts/%s.ttf",m_font.c_str())->_string.c_str(), m_fontsize), "0"));
	addChild(m_date = Label::createWithTTF(TTFConfig(String::createWithFormat("Resources/fonts/%s.ttf", m_font.c_str())->_string.c_str(), m_fontsize*2 / 3), "0"));
	m_date->setTextColor(Color4B(255, 255, 255,255));
	m_time->setTextColor(Color4B(255, 255, 255, 255));
	m_date->setAnchorPoint(Point(0, 0));
	m_time->setAnchorPoint(Point(0,0));
	float height = m_time->getContentSize().height;
	m_date->setPosition((m_time->getContentSize().width - m_date->getContentSize().width) / 2, height);
	m_timenow = time(0);
	tm* temp = localtime(&m_timenow);
	m_time->setString(String::createWithFormat("%02d:%02d", temp->tm_hour, temp->tm_min)->_string);
	m_date->setString(String::createWithFormat("%02d/%02d", temp->tm_mon + 1, temp->tm_mday)->_string);
	setContentSize(Size(m_time->getContentSize().width, m_date->getContentSize().height + height));
	scheduleUpdate();
	return true;
}
void Clock::update(float dt)
{
	time_t tempt;
	if (m_timenow!=(tempt = time(0)))
	{
		m_timenow = tempt;
		tm* temp = localtime(&m_timenow);
		m_time->setString(String::createWithFormat("%02d:%02d", temp->tm_hour, temp->tm_min)->_string);
		m_date->setString(String::createWithFormat("%02d/%02d", temp->tm_mon+1, temp->tm_mday)->_string);
	}
}
bool MainPortLayer::init()
{
	if (!Layer::init())
		return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/battleicon.plist", "Resources/img/battleicon.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/attackanime.plist", "Resources/img/attackanime.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/background.plist", "Resources/img/background.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/spritesheet.plist", "Resources/img/spritesheet.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/portmainsprites.plist", "Resources/img/portmainsprites.png");
	//background
	addChild(m_background = Sprite::createWithSpriteFrameName("portmainbackground.png"));
	addChild(m_buttomborder = Sprite::createWithSpriteFrameName("portmainbuttomborder.png"),3);
	addChild(m_leftborder = Sprite::createWithSpriteFrameName("portmainleftborder.png"));
	addChild(m_clockbackground = Sprite::createWithSpriteFrameName("portmainclockbackground.png"));
	addChild(m_clock = Clock::create(),4);
	addChild(m_commanderinfobar = CommanderInfoBar::create(), 5);
	addChild(m_buttommenuborder = Sprite::createWithSpriteFrameName("portmainbuttommenuborder.png"));
	addChild(m_buttonmenubackground = Sprite::createWithSpriteFrameName("portmainbuttommenubackground.png"));
	addChild(m_shipleaderVD = Sprite::create(),0);

	//commander, remind delete later
	Commander::New();
	m_commander = Commander::getInstance();
	Ship* sendai = ShipBuilder::newShip("SendaiKai2");
	sendai->Skills.push_back(AS_FullSalvo::getInstance());
	m_commander->ships.push_back(sendai);
	m_commander->currentShip = 0;


	//buttons
	addChild(m_attackbutton = AttackButton::create(),2);
	addChild(m_factorybutton = RoundButton::create("portmainfactorybutton.png"),2);
	addChild(m_modifybutton = RoundButton::create("portmainmodifybutton.png"),2);
	addChild(m_organizebutton = RoundButton::create("portmainorganizebutton.png"),2);
	addChild(m_missionbutton = RectButton::create("portmainbuttommenubuttonbackground.png", "portmainbuttommenumissionicon.png"),2);
	addChild(m_activitybutton = RectButton::create("portmainbuttommenubuttonbackground.png", "portmainbuttommenuactivityicon.png"),2);
	addChild(m_pokedexbutton = RectButton::create("portmainbuttommenubuttonbackground.png", "portmainbuttommenupokedexicon.png"),2);
	addChild(m_settingbutton = RectButton::create("portmainbuttommenubuttonbackground.png", "portmainbuttommenusettingicon.png"),2);
	

	m_attackbutton->ButtonClicked += std::bind(&MainPortLayer::attackbuttonClickedEventHandle, this, std::placeholders::_1, std::placeholders::_2);

	//listener
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(MainPortLayer::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(MainPortLayer::onTouchMoved, this);
	touchlistener->onTouchEnded = CC_CALLBACK_2(MainPortLayer::onTouchEnded, this);
	touchlistener->onTouchCancelled = CC_CALLBACK_2(MainPortLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
	return true;
}

void MainPortLayer::initLayout()
{
	//refresh data
	m_commanderinfobar->refresh(m_commander);
	m_shipleaderVD->setSpriteFrame(String::createWithFormat("%sVD.png", m_commander->ships[m_commander->currentShip]->shiptype.c_str())->_string);
	m_shipleaderVD->setAnchorPoint(Point(0.4, 1));
	m_shipleaderVD->setPosition(_contentSize.width, _contentSize.height);
	m_shipleaderVD->runAction(MoveBy::create(0.2, Point(-_contentSize.width/4, 0)));
	//background
	m_background->setPosition(_contentSize / 2);
	m_leftborder->setAnchorPoint(Point(0, 1));
	m_leftborder->setPosition(0, _contentSize.height);
	m_buttomborder->setAnchorPoint(Point(1, 0));
	m_buttomborder->setPosition(_contentSize.width, 0);
	m_clockbackground->setAnchorPoint(Point(0, 0));
	m_clockbackground->setPosition(0, 0);

	m_clock->setAnchorPoint(Point(0, 0));
	m_clock->setPosition(5,5);

	m_commanderinfobar->setAnchorPoint(Point(0, 1));
	m_commanderinfobar->setPosition(0, _contentSize.height);

	m_buttonmenubackground->setAnchorPoint(Point(1, 0));
	m_buttonmenubackground->setPosition(_contentSize.width, 0);
	m_buttommenuborder->setAnchorPoint(Point(0, 0));
	m_buttommenuborder->setPosition(_contentSize.width - m_buttonmenubackground->getContentSize().width - 34, 0);


	//buttons
	m_attackbutton->setPosition(Point(0, 263));
	m_attackbutton->runAction(MoveBy::create(0.2, Point(176, 0)));
	m_attackbutton->setScale(1.2);
	m_factorybutton->setPosition(m_attackbutton->getPosition() + Point(0, -150));
	m_factorybutton->runAction(MoveBy::create(0.2, Point(176, 0)));
	m_factorybutton->setScale(1.1);
	m_organizebutton->setPosition(m_attackbutton->getPosition() + Point(150, 0));
	m_organizebutton->runAction(MoveBy::create(0.2, Point(176, 0)));
	m_organizebutton->setScale(1.1);
	m_modifybutton->setPosition(m_attackbutton->getPosition() + Point(115, -115));
	m_modifybutton->runAction(MoveBy::create(0.2, Point(176, 0)));
	m_modifybutton->setScale(1.1);
	m_settingbutton->setAnchorPoint(Point(1, 0.1));
	m_settingbutton->setPosition(_contentSize.width-5, 0);
	m_pokedexbutton->setAnchorPoint(Point(1, 0.1));
	m_pokedexbutton->setPosition(m_settingbutton->getPosition() - Point(m_settingbutton->getContentSize().width+7, 0));
	m_activitybutton->setAnchorPoint(Point(1, 0.1));
	m_activitybutton->setPosition(m_pokedexbutton->getPosition() - Point(m_pokedexbutton->getContentSize().width+7, 0));
	m_missionbutton->setAnchorPoint(Point(1, 0.1));
	m_missionbutton->setPosition(m_activitybutton->getPosition() - Point(m_activitybutton->getContentSize().width+7, 0));
}



bool MainPortLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Button* temp;
	for (int i = 0; i < buttoncount; i++)
	{
		temp = (this->*m_buttons[i]); 
		temp->onTouchBegan(temp->convertTouchToNodeSpace(touch));
	}
	return true;
}
void MainPortLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Button* temp;
	for (int i = 0; i < buttoncount; i++)
	{
		temp = (this->*m_buttons[i]);
		temp->onTouchMoved(temp->convertTouchToNodeSpace(touch));
	}
}

void MainPortLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Button* temp;
	for (int i = 0; i < buttoncount; i++)
	{
		temp = (this->*m_buttons[i]);
		temp->onTouchEnded(temp->convertTouchToNodeSpace(touch));
	}
}
void MainPortLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Button* temp;
	for (int i = 0; i < buttoncount; i++)
	{
		temp = (this->*m_buttons[i]);
		temp->onTouchCancelled(temp->convertTouchToNodeSpace(touch));
	}
}
void MainPortLayer::onEnterTransitionDidFinish()
{
	initLayout();
}

void MainPortLayer::attackbuttonClickedEventHandle(Button* sender, EventArg* arg)
{
	auto scene = Scene::create();
	auto director = Director::getInstance();
	Player* A = new Player(m_commander->ships[m_commander->currentShip]);
	Ship* target = ShipBuilder::newShip("CruiserE");
	target->Skills.push_back(AS_FullSalvo::getInstance());
	Player_AI* B = new Player_AI(target);
	B->HardGrade = 3;
	auto gameLayer = BattleLayer_2P::createWithPlayer(A,B);
	scene->addChild(gameLayer);
	director->pushScene(scene);
	gameLayer->initLayout();
	gameLayer->gameStart();
}