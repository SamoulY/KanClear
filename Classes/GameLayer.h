#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"

class ChessBoard;
class HealthBar;
class HealthZeroEventArg;
class Block;
class BlockClearEventArg;
class BattleLayer;
class CoinBank;
class HealWorker;
class SkillButton;
class SkillExcuteEventArg;
class Player;
class Icon;

class BattleLayer :public cocos2d::Layer
{

};
class BattleLayer_2P :public cocos2d::Layer
{
public:
	static BattleLayer_2P* createWithPlayer(Player*, Player*);
	void initLayout();
	virtual void gameStart();	
	virtual void PlayerAttack(Player* attacker, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerLock(Player* locker, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerGetCoin(Player* target, std::vector<Block*>& blocks);
	virtual void PlayerGetMana(Player* manar, Player* target, std::vector<Block*>& blocks);
	virtual void PlayerHeal(Player* healer, Player* target, std::vector<Block*>& blocks);
protected:
	BattleLayer_2P(Player* A, Player* B) :m_playerA(A), m_playerB(B), m_background(0), m_VS(0), m_VSframe(0)
	{
	}	
	virtual bool init() override;


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

	cocos2d::Sprite* m_playerVDA;
	cocos2d::Layer* m_playerVDblinkA;
	cocos2d::Sprite* m_playerVDB;
	cocos2d::Layer* m_playerVDblinkB;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_playerIconA;
	cocos2d::Sprite* m_playerIconB;
	cocos2d::Sprite* m_VS;
	cocos2d::Sprite* m_VSframe;

	virtual void BlockClearedEventHandle(ChessBoard*, BlockClearEventArg*);
	virtual void HealthZeroEventHandle(HealthBar*, HealthZeroEventArg*);
	virtual void SkillExcuteEventHandle(SkillButton*, SkillExcuteEventArg*);
	

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
};



#endif