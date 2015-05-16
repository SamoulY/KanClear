#include "Buttons.h"

using namespace cocos2d;
RoundButton* RoundButton::create(const std::string& background, const std::string& icon)
{
	RoundButton* bu = new RoundButton();
	if (bu != 0&&!bu->init(background, icon))
	{
		bu->autorelease();
	}
	else
	{
		delete(bu);
		bu = 0;
	}
	return bu;
}
bool RoundButton::init(const std::string& background,const  std::string& icon)
{
	if (!Sprite::init())
		return false;
	addChild(m_base = Sprite::create());
	m_icon = 0;
	m_base->addChild(m_background = Sprite::createWithSpriteFrameName(background), 2);
	setContentSize(m_background->getContentSize());
	m_base->setPosition(_contentSize / 2);
	m_buttonr2 = m_background->getContentSize().width*m_background->getContentSize().width / 4;
	if (icon != "")
		m_base->addChild(m_icon = Sprite::createWithSpriteFrameName(icon), 3);
}
RectButton* RectButton::create(const std::string& background, const std::string& icon)
{
	RectButton* bu = new RectButton();
	if (bu != 0 && !bu->init(background, icon))
	{
		bu->autorelease();
	}
	else
	{
		delete(bu);
		bu = 0;
	}
	return bu;
}
bool RectButton::init(const std::string& background, const std::string& icon)
{
	if (!Sprite::init())
		return false;
	addChild(m_base = Sprite::create());
	m_icon = 0;
	m_base->addChild(m_background = Sprite::createWithSpriteFrameName(background), 2);
	setContentSize(m_background->getContentSize());
	m_base->setPosition(_contentSize / 2);
	m_width = m_background->getContentSize().width;
	m_height = m_background->getContentSize().height;
	if (icon != "")
		m_base->addChild(m_icon = Sprite::createWithSpriteFrameName(icon), 3);
}
bool AttackButton::init()
{
	if (!Sprite::init())
		return false;
	addChild(m_base = Sprite::create());
	m_base->addChild(m_gear = Sprite::createWithSpriteFrameName("portmaingear_l.png"), 2);
	setContentSize(m_gear->getContentSize());
	m_base->setPosition(_contentSize / 2);
	m_base->addChild(m_circle = Sprite::createWithSpriteFrameName("portmaingear_lp.png"), 3);
	m_gear->runAction(RepeatForever::create(RotateBy::create(60, 360)));
	m_circle->setVisible(false);
	Size circlesize = m_circle->getContentSize();
	m_buttonr2 = m_gear->getContentSize().width*m_gear->getContentSize().width / 4;
	m_base->addChild(m_waver = Sprite::createWithSpriteFrameName("portmainseawave_r.png"), 1);
	m_base->addChild(m_wavel = Sprite::createWithSpriteFrameName("portmainseawave_l.png"), 1);
	m_base->addChild(m_ship = Sprite::createWithSpriteFrameName("portmainattackship.png"), 9);
	m_base->addChild(m_label = Sprite::createWithSpriteFrameName("portmainattacklabel.png"), 10);
	m_ship->setPosition(0, circlesize.height /5);
	m_label->setPosition(0, -circlesize.height/5);
	m_waver->setPosition(circlesize.width / 5, -circlesize.height / 5);
	m_wavel->setPosition(-circlesize.width / 5, -circlesize.height / 5);
	return true;
}
void AttackButton::touch()
{
	RoundButton::touch();
	//m_ship->setSpriteFrame("portmainattackship_p.png");
	//m_label->setSpriteFrame("portmainattacklabel_p.png");
	//m_circle->setVisible(true);
	//m_wavel->setSpriteFrame("portmainseawave_lp.png");
	//m_waver->setSpriteFrame("portmainseawave_rp.png");
}
void AttackButton::untouch()
{
	RoundButton::untouch();
	//m_ship->setSpriteFrame("portmainattackship.png");
	//m_label->setSpriteFrame("portmainattacklabel.png");
	//m_circle->setVisible(false);
	//m_wavel->setSpriteFrame("portmainseawave_l.png");
	//m_waver->setSpriteFrame("portmainseawave_r.png");
}
