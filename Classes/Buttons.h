#ifndef _BUTTONS_H_
#define _BUTTONS_H_


#include "cocos2d.h"
#include "KEvent.h"

class Button :public cocos2d::Sprite
{
public:
	virtual void onTouchBegan(cocos2d::Point location) = 0;
	virtual void onTouchEnded(cocos2d::Point location) = 0;
	virtual void onTouchCancelled(cocos2d::Point location) = 0;
	virtual void onTouchMoved(cocos2d::Point location) = 0;
	KEvent<Button, EventArg> ButtonClicked;
protected:
	Button():ButtonClicked(this){}
};
class RoundButton :public Button
{
public:
	virtual void onTouchBegan(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (location.x*location.x + location.y*location.y <= m_buttonr2)
		{
			m_touched = true;
			touch();
		}
	}
	virtual void onTouchEnded(cocos2d::Point location) override
	{
		if (m_touched)
		{
			m_touched = false;
			untouch();
			location = location - m_base->getPosition();
			if (location.x*location.x + location.y*location.y <= m_buttonr2)
			{
				EventArg arg;
				ButtonClicked.func(arg);
			}
		}
	}
	virtual void onTouchCancelled(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (m_touched)
		{
			m_touched = false;
			untouch();
		}
	}
	virtual void onTouchMoved(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (m_touched&&location.x*location.x + location.y*location.y > m_buttonr2)
		{
			m_touched = false;
			untouch();
		}
	}
	static RoundButton* create(const std::string& background, const  std::string& icon = std::string(""));
protected:
	RoundButton() : m_touched(false), m_buttonr2(-1), m_base(0){};
	virtual bool init(const std::string& background, const  std::string& icon);
	virtual void touch()
	{
		if (m_base!=NULL)
			m_base->setScale(0.95);
	}
	virtual void untouch()
	{
		if (m_base != NULL)
			m_base->setScale(1);
	}
	float m_buttonr2;
	bool m_touched;
	cocos2d::Sprite* m_base;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_icon;
};
class RectButton :public Button
{
public:
	virtual void onTouchBegan(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (2 * abs(location.x)<m_width && 2 * abs(location.y)<m_height)
		{
			m_touched = true;
			touch();
		}
	}
	virtual void onTouchEnded(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (m_touched)
		{
			m_touched = false;
			untouch();
			if (2 * abs(location.x)<m_width && 2 * abs(location.y)<m_height)
			{
				EventArg arg;
				ButtonClicked.func(arg);
			}
		}
	}
	virtual void onTouchCancelled(cocos2d::Point location) override
	{
		if (m_touched)
		{
			m_touched = false;
			untouch();
		}
	}
	virtual void onTouchMoved(cocos2d::Point location) override
	{
		location = location - m_base->getPosition();
		if (m_touched&&2*abs(location.x)>m_width||2*abs(location.y)>m_height)
		{
			m_touched = false;
			untouch();
		}
	}
	static RectButton* create(const std::string& background, const std::string& icon = std::string(""));
protected:
	RectButton() : m_touched(false), m_width(-1), m_height(-1), m_base(0){};
	virtual bool init(const std::string& background, const std::string& icon);
	virtual void touch()
	{
		if (m_base != NULL)
			m_base->setScale(0.95);
	}
	virtual void untouch()
	{
		if (m_base != NULL)
			m_base->setScale(1);
	}
	float m_width,m_height;
	bool m_touched;
	cocos2d::Sprite* m_base;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_icon;
};
class AttackButton :public RoundButton
{
public:
	CREATE_FUNC(AttackButton);
protected:
	virtual bool init() override;
	virtual void touch() override;
	virtual void untouch() override;
	cocos2d::Sprite* m_gear;
	cocos2d::Sprite* m_waver;
	cocos2d::Sprite* m_wavel;
	cocos2d::Sprite* m_circle;
	cocos2d::Sprite* m_ship;
	cocos2d::Sprite* m_label;
};

#endif