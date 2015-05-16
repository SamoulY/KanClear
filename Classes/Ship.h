#ifndef _SHIP_H_
#define _SHIP_H_

#include <string>
#include <vector>


class ActiveSkill;
class Ship
{
public:
	int Hp;
	int FirePower;
	int TorpedoPower;
	int Def;
	int Level;
	std::vector<const ActiveSkill*> Skills;
	const std::string shiptype;
	const std::string shipdisplayname;
protected:
	Ship(int lv,int hp, int firepower, int torpedopower, int def, int maxhp, int  maxfirepower, int  maxtorpedopower, int  maxdef,const std::string& shiptype, const std::string& shipname) :m_maxHp(maxhp), m_maxFirePower(maxfirepower),
		m_maxTorpedoPower(maxtorpedopower), m_maxDef(maxdef), shiptype(shiptype), shipdisplayname(shipname), Level(lv), Hp(hp), FirePower(firepower), Def(def), TorpedoPower(torpedopower)
	{
	}
	const int m_maxHp;
	const int m_maxFirePower;
	const int m_maxTorpedoPower;
	const int m_maxDef;

	friend class ShipBuilder;
};
class ShipBuilder
{
public:
	static Ship* newShip(const std::string& shiptype);
	static Ship* loadShip();
};


#endif