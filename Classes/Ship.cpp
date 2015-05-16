#include "Ship.h"

Ship* ShipBuilder::newShip(const std::string& shiptype)
{
	Ship* result = new Ship(1,100, 50, 50, 50, 100, 50, 50, 50, shiptype, shiptype);
	return result;
}