#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_

#include "cocos2d.h"
#include "KEvent.h"

class Player;

class HealthBar : public cocos2d::Sprite
{
public:
	static HealthBar* create(int max, std::string& info);
	static HealthBar* create(int max)
	{
		auto arg = std::string("");
		return HealthBar::create(max, arg);
	}
	virtual void updateHealth(int target);
	virtual bool decreaseHealth(int target);
	virtual bool increaseHealth(int target);
	virtual int getCurrentHealth()
	{
		return m_current;
	}
	virtual cocos2d::Point healthToPoint(int target)
	{
		if (m_max<=0)
			return m_healthstrip->getPosition() + cocos2d::Point(0, m_healthmask->getContentSize().height / 4);
		return m_healthstrip->getPosition() + cocos2d::Point(m_healthmask->getContentSize().width*target / m_max, m_healthmask->getContentSize().height / 4);
	}	
	virtual cocos2d::Point healthToPoint()
	{
		if (m_max <= 0)
			return m_healthstrip->getPosition() + cocos2d::Point(0, m_healthmask->getContentSize().height / 4);
		return m_healthstrip->getPosition() + cocos2d::Point(m_healthmask->getContentSize().width*m_current / m_max, m_healthmask->getContentSize().height / 4);
	}
	virtual void setInfo(std::string& info);
	virtual cocos2d::Label* getInfoLabel()
	{
		return m_healthbarinfo;
	}
	KEvent<HealthBar, EventArg> HealthZeroEvent;
protected:
	cocos2d::ClippingNode* m_healthclip;
	cocos2d::Sprite* m_healthstrip;
	cocos2d::Sprite* m_healthborder;
	cocos2d::Sprite* m_healthmask;
	cocos2d::Label* m_healthbarinfo;
	int m_max;
	int m_current;
	virtual bool init(std::string& info);
	virtual void update(float dt) override;
	HealthBar(int max) :m_max(max), m_current(-1), m_healthbarinfo(0), HealthZeroEvent(this)
	{

	}
};


#endif