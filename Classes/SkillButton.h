#ifndef _SKILLBUTTON_H_
#define _SKILLBUTTON_H_

#include "cocos2d.h"
#include "KEvent.h"

class ActiveSkill;
class SkillExcuteEventArg
{
public:
	int index;
	SkillExcuteEventArg(int index) :index(index){};
};
class SkillButton :public cocos2d::Sprite
{
public:
	int getCurrentMana()
	{
		return m_mana;
	}
	virtual cocos2d::Point manaToPoint(int target)
	{
		if (m_manamax <= 0)
			return m_skillstrip->getPosition() + cocos2d::Point(m_skillclip->getPositionX(), m_skillclip->getPositionY());
		return m_skillstrip->getPosition() + cocos2d::Point(m_skillclip->getPositionX(), m_skillclip->getPositionY()+m_skillmask->getContentSize().height*target / m_manamax);
	}
	virtual cocos2d::Point manaToPoint()
	{
		if (m_manamax <= 0)
			return m_skillstrip->getPosition() + cocos2d::Point(m_skillclip->getPositionX(), m_skillclip->getPositionY());
		return m_skillstrip->getPosition() + cocos2d::Point(m_skillclip->getPositionX(), m_skillclip->getPositionY() + m_skillmask->getContentSize().height*m_mana / m_manamax);
	}
	virtual float getStripWidth()
	{
		return m_skillstrip->getContentSize().width;
	}
	std::string getCurrentSkillName()
	{
		int index = getCurrentSkillIndex();
		return index >= 0 ? m_skillsnames[index] : "";
	}
	int getCurrentSkillIndex()
	{
		return m_mana/30-1;
	}
	static SkillButton* create(const std::vector<const ActiveSkill*>&);
	virtual bool increaseMana(int mana);
	virtual bool decreaseMana(int mana);
	virtual void onTouchBegan(cocos2d::Point location);
	virtual void onTouchEnded(cocos2d::Point location);
	virtual void onTouchCancelled(cocos2d::Point location);
	virtual void onTouchMoved(cocos2d::Point location);
	virtual void excuteSkill();
	KEvent<SkillButton, SkillExcuteEventArg> SkillExcuteEvent;
protected:
	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void check();
	SkillButton(const std::vector<const ActiveSkill*>& skills);
	std::vector<std::string> m_skillsnames;
	cocos2d::ClippingNode* m_skillclip;
	cocos2d::Sprite* m_skilliconmask;
	cocos2d::ClippingNode* m_skilliconclip;
	cocos2d::Sprite* m_skillstrip;
	cocos2d::Sprite* m_skillmask;
	cocos2d::Sprite* m_skillgear;
	cocos2d::Sprite* m_currentskill;
	cocos2d::Sprite* m_currentskillrotater;
	cocos2d::Sprite* m_skillcircle1;
	cocos2d::Sprite* m_skillcircle2;
	float m_buttonr2;
	float m_buttonr;
	const int m_skillcount;
	int m_currentskillindex;
	int m_mana;
	int m_manamax;
	bool m_touched;
};

class SkillButtonSimple :public SkillButton
{
public:
	static SkillButtonSimple* create(const std::vector<const ActiveSkill*>&);
	virtual void onTouchBegan(cocos2d::Point location) override{};
protected:
	SkillButtonSimple(const std::vector<const ActiveSkill*>& skills) :SkillButton(skills){}
	virtual bool init() override;
	virtual void check() override{};
};
#endif