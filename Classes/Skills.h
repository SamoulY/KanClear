#ifndef _SKILLS_H_
#define _SKILLS_H_


#include "cocos2d.h"

class Player;
class BattleLayer_2P;

enum PassiveSkillExcuteTime
{
	beforegame = 0,
	beforecomputedamage = 1
};

class ActiveSkill
{
public:
	virtual void excute(Player* skiller, Player* target, BattleLayer_2P* layer) const = 0;
	std::string getName() const
	{
		return m_name;
	}
protected:
	ActiveSkill(std::string name) :m_name(name){};
	const std::string m_name;
};
class PassiveSkill
{
public:
	PassiveSkillExcuteTime excuteTime;
};
class AS_FullSalvo :public ActiveSkill
{
public:
	virtual void excute(Player* skiller, Player* target, BattleLayer_2P* layer) const override;
	static const ActiveSkill* getInstance()
	{
		return m_instance == 0 ? m_instance = new AS_FullSalvo():m_instance;
	}
protected:
	AS_FullSalvo():ActiveSkill("FullSalvo"){};
	static AS_FullSalvo* m_instance;
};

#endif