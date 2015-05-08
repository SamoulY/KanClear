#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_

#include "cocos2d.h"
#include "EventHandler.h"

class Player;

class HealthZeroEventArg
{
public:
	HealthZeroEventArg()
	{

	}
	~HealthZeroEventArg()
	{

	}
};
class HealthBar : public cocos2d::Sprite
{
public:
	static HealthBar* create(int max, std::string& info = std::string(""));
	virtual void updateHealth(int target);
	virtual bool decreaseHealth(int target);
	virtual bool increaseHealth(int target);
	virtual int getCurrentHealth()
	{
		return m_current;
	}
	virtual cocos2d::Point healthToPoint(int target)
	{
		return m_healthstrip->getPosition() + cocos2d::Point(m_healthmask->getContentSize().width*target / m_max, m_healthmask->getContentSize().height / 4);
	}	
	virtual cocos2d::Point healthToPoint()
	{
		return m_healthstrip->getPosition() + cocos2d::Point(m_healthmask->getContentSize().width*m_current / m_max, m_healthmask->getContentSize().height / 4);
	}
	virtual void start(int target= -1);
	virtual void setInfo(std::string& info);
	virtual cocos2d::Label* getInfoLabel()
	{
		return m_healthbarinfo;
	}
	KEvent<HealthBar, HealthZeroEventArg> HealthZeroEvent;
protected:
	cocos2d::ClippingNode* m_healthclip;
	cocos2d::Sprite* m_healthstrip;
	cocos2d::Sprite* m_healthborder;
	cocos2d::Sprite* m_healthmask;
	cocos2d::Label* m_healthbarinfo;
	int m_max;
	int m_current;
	int m_temp;
	virtual bool init(std::string& info);
	virtual void update(float dt) override;
	HealthBar(int max) :m_max(max), m_current(max), m_temp(max), m_healthbarinfo(0), HealthZeroEvent(this)
	{

	}
};


#endif