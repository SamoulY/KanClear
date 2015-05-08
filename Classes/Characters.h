#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <string>
#include <vector>
#include "Skills.h"

class Character
{
public:
	int HP;
	int Mana;
	int ATK;
	int DEF;
	int Level;
	std::string Name;
	std::vector<const ActiveSkill* const> Skills;
protected:
	Character(int hp, int mana, int atk, int def, int level, std::string name) :HP(hp), Mana(mana), ATK(atk), DEF(def), Level(level), Name(name)
	{

	}
};

class SendaiKai2Character :public Character
{
public:
	SendaiKai2Character() :Character(100, 100, 50, 50, 1, "SendaiKai2")
	{
		Skills.push_back(AS_FullSalvo::getInstance());
	}
};
class CruiserECharacter :public Character
{
public:
	CruiserECharacter() :Character(100, 100, 50, 50, 1, "CruiserE")
	{
		Skills.push_back(AS_FullSalvo::getInstance());
	}
};



#endif