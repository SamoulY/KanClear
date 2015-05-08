#include "HealthBar.h"
#include "Characters.h"

using namespace cocos2d;

HealthBar* HealthBar::create(int max, std::string& info)
{
	HealthBar* gl = new HealthBar(max);
	if (gl&&gl->init(info))
	{
		gl->autorelease();
	}
	else
	{
		delete(gl);
		gl = nullptr;
	}
	return gl;
}

bool HealthBar::init(std::string& info)
{
	if (!Sprite::initWithSpriteFrameName("healthbarbackground.png"))
		return false;
	addChild(m_healthborder = Sprite::createWithSpriteFrameName("battleplayerflame.png"), 10);
	m_healthborder->setPosition(_contentSize/2);
	addChild(m_healthclip = ClippingNode::create(), 5);
	m_healthclip->addChild(m_healthstrip = Sprite::createWithSpriteFrameName("healthbarstrip.png"));
	m_healthclip->setPosition(Point((_contentSize.width - m_healthstrip->getContentSize().width) / 2, 0));
	m_healthclip->setStencil(m_healthmask = Sprite::createWithSpriteFrameName("healthbarstrip.png"));
	m_healthmask->setAnchorPoint(Point(0, 0));
	m_healthclip->setInverted(true);
	m_healthstrip->setAnchorPoint(Point(0, 0));
	//m_healthmask->setPosition(m_healthmask->getContentSize().width, 0);
	m_healthstrip->setColor(Color3B(0, 255, 0));

	addChild(m_healthbarinfo = Label::createWithTTF(TTFConfig("Resources/fonts/Marker Felt.ttf", 20), info),20);
	m_healthbarinfo->setAnchorPoint(Point(1, 0.5));
	m_healthbarinfo->setPosition(_contentSize.width*0.98, _contentSize.height * 3 / 4);

	scheduleUpdate();
	return true;
}
void HealthBar::start(int target)
{
	m_current = 0;
	if (target == -1)
		updateHealth(m_max);
	else
		updateHealth(target);
}
void HealthBar::setInfo(std::string& info)
{
	m_healthbarinfo->setString(info);
}

void HealthBar::updateHealth(int target)
{
	if (target != m_current)
	{
		float speed = ((float)target - m_current) / m_max;
		float offset = m_healthmask->getContentSize().width*speed;
		m_healthmask->runAction(MoveBy::create(0.5, Point(offset, 0)));
		m_current = target;
		if (m_current == 0)
			HealthZeroEvent.func(HealthZeroEventArg());
	}
}
bool HealthBar::increaseHealth(int target)
{
	if (target > 0&&m_current!=m_max)
	{
		int last = m_current;
		m_current += target;
		if (m_current > m_max)
			m_current = m_max;
		float offset = m_healthmask->getContentSize().width*(m_current - last) / m_max;
		m_healthmask->runAction(MoveBy::create(0.5, Point(offset, 0)));
		if (m_current == 0)
			HealthZeroEvent.func(HealthZeroEventArg());
		return true;
	}
	return false;
}
bool HealthBar::decreaseHealth(int target)
{
	if (target > 0&&m_current!=0)
	{
		int last = m_current;
		m_current -= target;
		if (m_current < 0)
			m_current = 0;
		float offset = m_healthmask->getContentSize().width*(m_current - last) / m_max;
		m_healthmask->runAction(MoveBy::create(0.5, Point(offset, 0)));
		if (m_current==0)
			HealthZeroEvent.func(HealthZeroEventArg());
		return true;
	}
	return false;
}
void HealthBar::update(float dt)
{
	float ch = m_healthmask->getPositionX() / m_healthmask->getContentSize().width;
	if (ch > 0.66)
		m_healthstrip->setColor(Color3B(0, 255, 0));
	else if (ch > 0.25)
		m_healthstrip->setColor(Color3B(255, 255, 0));
	else
		m_healthstrip->setColor(Color3B(255, 0, 0));

}