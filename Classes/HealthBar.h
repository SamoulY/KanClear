#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_

#include "cocos2d.h"

class Player;

class HealthBar : public cocos2d::Sprite
{
public:
	static HealthBar* create(int max);
	virtual void updateHealth(int target);
	virtual int getCurrentHealth()
	{
		return m_current;
	}
	virtual cocos2d::Point healthToPoint(int target)
	{
		return m_healthstrip->getPosition() + cocos2d::Point(m_healthmask->getContentSize().width*target / m_max, m_healthmask->getContentSize().height / 4);
	}
protected:
	cocos2d::ClippingNode* m_healthclip;
	cocos2d::Sprite* m_healthstrip;
	cocos2d::Sprite* m_healthborder;
	cocos2d::Sprite* m_healthmask;
	int m_max;
	int m_current;
	int m_temp;
	virtual bool init() override;
	virtual void update(float dt) override;
	HealthBar(int max) :m_max(max), m_current(max), m_temp(max)
	{

	}
};


#endif