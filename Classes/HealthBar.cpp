#include "HealthBar.h"
#include "Characters.h"

using namespace cocos2d;

HealthBar* HealthBar::create(int max)
{
	HealthBar* gl = new HealthBar(max);
	if (gl&&gl->init())
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

bool HealthBar::init()
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
	m_healthmask->setPosition(m_healthmask->getContentSize().width, 0);
	m_healthstrip->setColor(Color3B(0, 255, 0));
	scheduleUpdate();
	return true;
}
void HealthBar::updateHealth(int target)
{
	if (target != m_current)
	{
		float speed = ((float)target - m_current) / m_max;
		float offset = m_healthmask->getContentSize().width*speed;
		m_healthmask->runAction(MoveBy::create(abs(speed*3), Point(offset, 0)));
		m_current = target;
	}
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