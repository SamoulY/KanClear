#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "Characters.h"

class ChessBoardLayer;
class HealthBar;
class BlockClearEventArg;
class BattleLayer;
class Player :public Character
{
public:
	int currentHp;
	int currentMana;
	int attacking;
	Player(Character* c);
	virtual void gameStart();
	virtual HealthBar* getHealthBar();
	virtual ChessBoardLayer* getChessBoard();
	virtual cocos2d::Sprite* getIcon();
protected:
	Character* m_character;
	HealthBar* m_hpbar;
	ChessBoardLayer* m_controller;
	cocos2d::Sprite* m_icon;
	friend class BattleLayer;
};
class Player_AI :public Player
{
public:
	Player_AI(Character* c) :Player(c), HardGrade(1), m_currentschedule(0)
	{

	}
	virtual void gameStart();
	float HardGrade;
protected:
	int m_currentschedule;
	virtual void move(float dt);
};
class BattleLayer :public cocos2d::Layer
{
protected:
	void AttackPlayer(Player* target, int damage);
	void HealPlayer(Player* target, int number);
	void LockPlayer(Player* target, int number);
};
class BattleLayer_2P :public BattleLayer
{
public:
	static BattleLayer_2P* createWithPlayer(Player*, Player*);
	void initLayout();
	void gameStart()
	{
		m_playerA->gameStart();
		m_playerB->gameStart();
	}
protected:
	BattleLayer_2P(Player* A, Player* B) :m_playerA(A), m_playerB(B), m_background(0), m_VS(0), m_VSframe(0), m_attackanimeCountA(0), m_attackanimeCountB(0)
	{
	}	
	int m_attackanimeCountA;
	int m_attackanimeCountB;
	void PlayerAttack(Player* attacker, Player* target, BlockClearEventArg* arg);
	void PlayerLock(Player* locker, Player* target, BlockClearEventArg* arg);
	virtual bool init() override;
	Player* m_playerA;
	Player* m_playerB;
	HealthBar* m_hpbarA;
	ChessBoardLayer* m_controllerA;
	HealthBar* m_hpbarB;
	ChessBoardLayer* m_controllerB;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_playerIconA;
	cocos2d::Sprite* m_playerIconB;
	cocos2d::Sprite* m_VS;
	cocos2d::Sprite* m_VSframe;

	virtual void BlockClearedEventHandle(void*, BlockClearEventArg*);
};



#endif