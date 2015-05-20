#include "Stages.h"
#include "Player.h"
#include "Ship.h"
#include "Skills.h"


Operation* OperationFactory::getOperation(int code)
{
	return new Operation(code, "teststage", 3);
}
Stage* StageFactory::getStage(int operationcode, int stageindex)
{
	Ship* target = ShipBuilder::newShip("CruiserE");
	target->Skills.push_back(AS_FullSalvo::getInstance());
	Stage* result = new Stage(new Player_AI(target, 3));
	return result;
}
Stage::~Stage()
{
	delete m_enemy;
}