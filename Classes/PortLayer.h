#ifndef _PORTLAYER_H_
#define _PORTLAYER_H_

#include "cocos2d.h"

class Button;
class MainPortLayer;
class CommanderInfoBar;
class EventArg;
class Commander;
typedef Button* MainPortLayer::*MainPortButton;


class MainPortLayer :public cocos2d::Layer
{
public:
	CREATE_FUNC(MainPortLayer);
	void initLayout();
protected:
	bool init() override;
	MainPortLayer():m_background(0){};
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_buttomborder;
	cocos2d::Sprite* m_leftborder;
	cocos2d::Sprite* m_clockbackground;
	cocos2d::Sprite* m_clock;
	cocos2d::Sprite* m_buttommenuborder;
	cocos2d::Sprite* m_buttonmenubackground;
	cocos2d::Sprite* m_shipleaderVD;
	CommanderInfoBar* m_commanderinfobar;
	
	Commander* m_commander;

	Button* m_attackbutton;
	Button* m_factorybutton;
	Button* m_modifybutton;
	Button* m_organizebutton;
	Button* m_missionbutton;
	Button* m_activitybutton;
	Button* m_pokedexbutton;
	Button* m_settingbutton;
	static const int buttoncount = 8;
	static MainPortButton m_buttons[];

	virtual void attackbuttonClickedEventHandle(Button*, EventArg*);
	virtual void onEnterTransitionDidFinish() override;

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

};

#endif