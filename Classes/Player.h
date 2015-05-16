#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Ship.h"
#include <string>
#include <vector>

class ChessBoard;
class HealthBar;
class HealthZeroEventArg;
class Block;
class BlockClearEventArg;
class BattleLayer;
class CoinBank;
class HealWorker;
class SkillButton;
class Icon;
class ActiveSkill;

class Player 
{
public:
	int attacking;
	bool healthlocked;
	Player(Ship* c);
	virtual void gameStart();
	virtual void gameOver();
	virtual void gameWin();
	virtual void gameStop();
	virtual void gameResume();
	virtual HealthBar* getHealthBar();
	virtual ChessBoard* getChessBoard();
	virtual Icon* getIcon();
	virtual HealWorker* getHealWorker();
	virtual SkillButton* getSkillButton();
	virtual CoinBank* getCoinBank();
	int getMaxHP() const
	{
		return m_ship->Hp;
	}
	int getFirePower() const
	{
		return m_ship->FirePower;
	}
	int getTorpedoPower() const
	{
		return m_ship->TorpedoPower;
	}
	int getDef() const
	{
		return m_ship->Def;
	}
	std::string getShipType() const
	{
		return m_ship->shiptype;
	}
	std::string getShipName() const
	{
		return m_ship->shipdisplayname;
	}
	const std::vector<const ActiveSkill*>& getSkills() const
	{
		return m_ship->Skills;
	}
protected:
	Ship* m_ship;
	HealthBar* m_hpbar;
	ChessBoard* m_controller;
	HealWorker* m_healworker;
	SkillButton* m_skillbutton;
	CoinBank* m_coinbank;
	Icon* m_icon;
};
class Player_AI :public Player
{
public:
	Player_AI(Ship* c) :Player(c), HardGrade(1), m_currentschedule(0)
	{

	}
	virtual void gameStart();
	virtual SkillButton* getSkillButton() override;
	float HardGrade;
protected:
	int m_currentschedule;
	virtual void move(float dt);
};



#endif