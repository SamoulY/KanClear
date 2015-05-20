#ifndef _STAGES_H_
#define _STAGES_H_

#include <vector>
#include <string>
class Player;
class StageFactory;
class Stage
{
public:
	virtual Player* getEnemy()
	{
		return m_enemy;
	}
	~Stage();
private:
	Stage(Player* enemy) :m_enemy(enemy){}
	Player* m_enemy;
	friend class StageFactory;
};
class StageFactory
{
public:
	static Stage* getStage(int operationcode,int stageindex);
};
class OperationFactory;
class Operation
{
public:
	const std::string& getName() const
	{
		return m_name;
	}
	const int getStageCount()
	{
		return stage_count;
	}
	const Stage* getStage(int i)
	{
		if (i >= stage_count)
			return 0;
		return StageFactory::getStage(m_code, i);
	}
private:
	Operation(int code, std::string name, int stagecount):m_code(code),m_name(name),stage_count(stagecount){}
	int m_code;
	int stage_count;
	std::string m_name;
	friend class OperationFactory;
};
class OperationFactory
{
public:
	static Operation* getOperation(int code);
};


#endif