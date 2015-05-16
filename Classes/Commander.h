#ifndef _COMMANDER_H_
#define _COMMANDER_H_

#include <string>
#include <vector>

class Ship;
class Commander
{
public:
	int lv;
	int currentexp;
	int maxexp;
	std::string name;
	int shipscount;
	int maxshipscount;
	int equipmentcount;
	int maxequipmentcount;
	int coincount;
	int quickbuildcount;
	int modifymaterialcount;
	int developmentcmaterialcount;
	int currentShip;
	std::vector<Ship*> ships;
	static Commander* getInstance()
	{
		return m_instance;
	}
	bool Save(){}
	static bool New();
	static bool Load(); 
	Commander() :lv(1), currentexp(0), maxexp(lv * 50), shipscount(0), equipmentcount(0), coincount(0), quickbuildcount(0), modifymaterialcount(0), developmentcmaterialcount(0), maxshipscount(100), maxequipmentcount(500), currentShip(-1), name("Customer")
	{
	}
protected:
	static Commander* m_instance;
};

#endif