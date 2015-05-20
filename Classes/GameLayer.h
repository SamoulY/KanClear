#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"

class ChessBoard;
class HealthBar;
class EventArg;
class Block;
class BlockClearEventArg;
class CoinBank;
class HealWorker;
class SkillButton;
class SkillExcuteEventArg;
class Player;
class Icon;
class Operation;
class Stage;
class BattleVerticalDraw;

class BattleLayer_2P :public cocos2d::Layer
{
public:
	static BattleLayer_2P* createByOperation(int operationcode);
	virtual void gameStart();	
	virtual void PlayerAttack(Player* attacker, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerLock(Player* locker, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerGetCoin(Player* target, std::vector<Block*>& blocks);
	virtual void PlayerGetMana(Player* manar, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerHeal(Player* healer, Player* target, std::vector<Block*>& blocks);
	virtual void addPlayerA(Player* a);
	virtual void addPlayerB(Player* b);
protected:
	BattleLayer_2P(int operationcode) :m_operationcode(operationcode), m_operation(0), m_currentstage(0), m_currrentstageindex(-1), m_playerA(0), m_playerB(0)
	{
	}	
	~BattleLayer_2P(){};
	virtual bool init() override;

	int m_operationcode;
	Operation* m_operation;
	int m_currrentstageindex;
	Stage* m_currentstage;

	Player* m_playerA;
	Player* m_playerB;
	HealthBar* m_hpbarA;
	ChessBoard* m_controllerA;
	HealthBar* m_hpbarB;
	ChessBoard* m_controllerB;
	CoinBank* m_bankA;
	CoinBank* m_bankB;
	HealWorker* m_healworkerA;
	HealWorker* m_healworkerB;
	SkillButton* m_skillbuttonA;
	SkillButton* m_skillbuttonB;

	BattleVerticalDraw* m_playerVDA;
	BattleVerticalDraw* m_playerVDB;
	cocos2d::Sprite* m_playerIconA;
	cocos2d::Sprite* m_playerIconB;

	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_VS;
	cocos2d::Sprite* m_VSframe;
	cocos2d::Sprite* m_transmaskt;
	cocos2d::Sprite* m_transmaskb;
	cocos2d::Sprite* m_transtextbg;


	virtual void changestage(int index);
	virtual void nextstage()
	{
		changestage(m_currrentstageindex+1);
	}
	virtual void removePlayerB();
	virtual void stagestart();

	virtual void BlockClearedEventHandle(ChessBoard*, BlockClearEventArg*);
	virtual void HealthZeroEventHandle(HealthBar*, EventArg*);
	virtual void SkillExcuteEventHandle(SkillButton*, SkillExcuteEventArg*);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
};



#endif