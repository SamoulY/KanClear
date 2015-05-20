#include "GameLayer.h"
#include "Block.h"
#include "ChessBoard.h"
#include "HealthBar.h"
#include "CoinBank.h"
#include "HealWorker.h"
#include "SkillButton.h"
#include "Icon.h"
#include "Player.h"
#include "Skills.h"
#include "Ship.h"
#include "Stages.h"
#include "Commander.h"
#include <cmath>

#define A_CONTROLLER_W 420
#define A_CONTROLLER_H 372
#define B_CONTROLLER_W 246
#define B_CONTROLLER_H 218
#define A_CONTROLLER_X 23
#define A_CONTROLLER_Y 8
#define B_CONTROLLER_X 506
#define B_CONTROLLER_Y 161
#define B_HPBAR_X _contentSize.width
#define B_HPBAR_Y _contentSize.height
#define B_ICON_X _contentSize.width
#define B_ICON_Y _contentSize.height
#define B_SKILLBUTTON_X (B_CONTROLLER_X + B_CONTROLLER_W)
#define B_SKILLBUTTON_Y (B_CONTROLLER_Y + B_CONTROLLER_H / 2)
#define B_VD_X _contentSize.width
#define B_VD_Y _contentSize.height
#define STAGECHANGEOFFSET (_contentSize.width/2)

using namespace cocos2d;


class BattleVerticalDraw:public cocos2d::Sprite
{
public:
	void splash(float time);
	static BattleVerticalDraw* create(const std::string& shiptype);

protected:
	bool init(const std::string& shiptype);
	BattleVerticalDraw(){};
	std::string m_picname;
	cocos2d::Layer* m_blink;
	cocos2d::ClippingNode* m_clip;
	cocos2d::Sprite* m_mask;
};
BattleVerticalDraw* BattleVerticalDraw::create(const std::string& shiptype)
{
	BattleVerticalDraw* vd = new BattleVerticalDraw();
	if (vd != 0 && vd->init(shiptype))
	{
		vd->autorelease();
	}
	else
	{
		delete vd;
		vd = 0;
	}
	return vd;
}
bool BattleVerticalDraw::init(const std::string& shiptype)
{
	if (!initWithSpriteFrameName(String::createWithFormat("%sVD.png", shiptype.c_str())->_string))
		return false;
	setOpacity(230);
	addChild(m_clip = ClippingNode::create());
	m_clip->setStencil(m_mask = Sprite::createWithSpriteFrame(getSpriteFrame()));
	m_mask->setAnchorPoint(Point(0, 0));
	m_clip->setAlphaThreshold(0);
	m_clip->addChild(m_blink = CCLayerColor::create(Color4B(255, 255, 255, 255), _contentSize.width, _contentSize.height));
	m_blink->setOpacity(0);
	return true;
}
void BattleVerticalDraw::splash(float dt)
{
	m_blink->stopAllActions();
	m_blink->runAction(Sequence::create(EaseSineIn::create(FadeIn::create(dt*0.2)), FadeOut::create(dt*0.8), NULL));
}

void BattleLayer_2P::gameStart()
{
	m_transmaskt->setPosition(_contentSize.width / 2, _contentSize.height);
	m_transmaskb->setPosition(_contentSize.width / 2, 0);
	m_transtextbg->setPosition(_contentSize / 2);
	Commander* com = Commander::getInstance();
	addPlayerA(new Player(com->ships[com->currentShip]));
	m_transtextbg->setScaleY(0);
	m_transtextbg->runAction(Sequence::create(DelayTime::create(0.3), ScaleTo::create(0.2, 1), DelayTime::create(1.5), ScaleTo::create(0.2, 1,0),Hide::create(), NULL));
	m_transmaskt->runAction(Sequence::create(DelayTime::create(2), MoveBy::create(0.5, Point(0, m_transmaskt->getContentSize().height)), NULL));
	m_transmaskb->runAction(Sequence::create(DelayTime::create(2), MoveBy::create(0.5, Point(0, -m_transmaskb->getContentSize().height)), NULL));
	runAction(Sequence::create(DelayTime::create(2.5), CallFunc::create(std::bind(&BattleLayer_2P::nextstage,this)), NULL));
}
BattleLayer_2P* BattleLayer_2P::createByOperation(int operationcode)
{
	BattleLayer_2P* gl = new BattleLayer_2P(operationcode);
	if (gl&&gl->init())
	{
		gl->autorelease();
	}
	else
	{
		delete(gl);
		gl = nullptr;
	}
	return gl;
}
bool BattleLayer_2P::init()
{
	if (!Layer::init())
		return false;
	if ((m_operation = OperationFactory::getOperation(m_operationcode)) == 0)
		return false;
	int i;
	auto attackanime = Animation::create();
	for (i =1; i <= 12; i++)
		attackanime->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CCString::createWithFormat("attackA_%d.png", i)->_string));
	for (i = 13; i <= 15; i++)
	{
		attackanime->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CCString::createWithFormat("attackA_%d.png", i)->_string));
		attackanime->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CCString::createWithFormat("attackA_%d.png", i)->_string));
	}
	attackanime->setDelayPerUnit(0.03);
	attackanime->setRestoreOriginalFrame(false);
	AnimationCache::getInstance()->addAnimation(attackanime, "attack");
	

	addChild(m_background = Sprite::createWithSpriteFrameName("battlemain.png"), 0);
	addChild(m_VS = Sprite::createWithSpriteFrameName("VS.png"), 10);
	addChild(m_VSframe = Sprite::createWithSpriteFrameName("Gear.png"), 9);
	addChild(m_transmaskt = Sprite::createWithSpriteFrameName("battletranmask_t.png"), 100);
	addChild(m_transmaskb = Sprite::createWithSpriteFrameName("battletranmask_b.png"), 99);
	addChild(m_transtextbg = Sprite::createWithSpriteFrameName("battletrantextbg.png"), 101);

	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(BattleLayer_2P::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(BattleLayer_2P::onTouchMoved, this);
	touchlistener->onTouchEnded = CC_CALLBACK_2(BattleLayer_2P::onTouchEnded, this);
	touchlistener->onTouchCancelled = CC_CALLBACK_2(BattleLayer_2P::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
	_eventDispatcher->pauseEventListenersForTarget(this);


	m_background->setPosition(_contentSize / 2);
	m_VS->setPosition(_contentSize.width / 2, _contentSize.height-32);
	m_VSframe->setPosition(m_VS->getPosition());
	if (m_VSframe->getNumberOfRunningActions() == 0)
		m_VSframe->runAction(RepeatForever::create(RotateBy::create(36, 360)));

	m_transmaskt->setAnchorPoint(Point(0.5, 1));
	m_transmaskb->setAnchorPoint(Point(0.5, 0));
	return true;
}
void BattleLayer_2P::changestage(int index)
{
	if (m_currentstage != 0)
		delete m_currentstage;
	m_currentstage = StageFactory::getStage(m_operationcode, index);
	m_currrentstageindex = index;
	addPlayerB(m_currentstage->getEnemy());
	Action* stagechangeaction_playerb = Sequence::create(DelayTime::create(2.5), EaseSineOut::create(MoveBy::create(0.7, Point(-STAGECHANGEOFFSET, 0))), NULL);
	m_playerIconB->runAction(stagechangeaction_playerb->clone());
	m_controllerB->runAction(stagechangeaction_playerb->clone());
	m_skillbuttonB->runAction(stagechangeaction_playerb->clone());
	m_hpbarB->runAction(stagechangeaction_playerb->clone());

	Sprite* temp = Sprite::createWithSpriteFrameName("battlesearchcircle.png");
	addChild(temp);
	temp->setScale(0);
	temp->setPosition(Point(_contentSize) - Point(30, 30));
	temp->runAction(Sequence::create(DelayTime::create(0.4), Spawn::create(ScaleTo::create(1, 1),EaseSineIn::create(FadeOut::create(1)),NULL), CallFunc::create(std::bind(&Sprite::removeFromParent, temp)), NULL));
	temp = Sprite::createWithSpriteFrameName("battlesearchcircle.png");
	addChild(temp);
	temp->setScale(0);
	temp->setPosition(Point(_contentSize) - Point(90, 60));
	temp->runAction(Sequence::create(DelayTime::create(0.8), Spawn::create(ScaleTo::create(1, 1), EaseSineIn::create(FadeOut::create(1)), NULL), CallFunc::create(std::bind(&Sprite::removeFromParent, temp)), NULL));
	temp = Sprite::createWithSpriteFrameName("battlesearchcircle.png");
	addChild(temp);
	temp->setScale(0);
	temp->setPosition(Point(_contentSize) - Point(150, 45));
	temp->runAction(Sequence::create(DelayTime::create(1.2), Spawn::create(ScaleTo::create(1, 1), EaseSineIn::create(FadeOut::create(1)), NULL), CallFunc::create(std::bind(&Sprite::removeFromParent, temp)), NULL));
	runAction(Sequence::create(DelayTime::create(3), CallFunc::create(std::bind(&BattleLayer_2P::stagestart,this)),NULL));
}

void BattleLayer_2P::stagestart()
{
	m_playerA->gameStart();
	m_playerB->gameStart();
	_eventDispatcher->resumeEventListenersForTarget(this);
}
void BattleLayer_2P::addPlayerA(Player* a)
{
	m_playerA = a;
	addChild(m_controllerA = m_playerA->getChessBoard(), 3);
	m_controllerA->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_hpbarA = m_playerA->getHealthBar(), 5);
	m_hpbarA->HealthZeroEvent += std::bind(&BattleLayer_2P::HealthZeroEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_playerIconA = m_playerA->getIcon(), 10);
	addChild(m_healworkerA = m_playerA->getHealWorker(), 11);
	addChild(m_skillbuttonA = m_playerA->getSkillButton(), 5);
	m_skillbuttonA->SkillExcuteEvent += std::bind(&BattleLayer_2P::SkillExcuteEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_bankA = m_playerA->getCoinBank(), 20);
	addChild(m_playerVDA = BattleVerticalDraw::create(m_playerA->getShipType()), 95);
	m_hpbarA->setAnchorPoint(Point(0, 1));
	m_hpbarA->setPosition(0, _contentSize.height); 
	m_playerIconA->setAnchorPoint(Point(0, 1));
	m_playerIconA->setPosition(0, _contentSize.height);
	m_healworkerA->setAnchorPoint(Point(0, 0));
	m_healworkerA->setOriginPosion(-m_healworkerA->getContentSize().width, convertToNodeSpace(m_hpbarA->convertToWorldSpace(m_hpbarA->healthToPoint(0))).y, true);
	m_controllerA->setContentSize(Size(A_CONTROLLER_W, A_CONTROLLER_H));
	m_controllerA->setAnchorPoint(Point(0, 0));
	m_controllerA->setPosition(A_CONTROLLER_X, A_CONTROLLER_Y);
	m_controllerA->refreshBackGround();
	m_bankA->setAnchorPoint(Point(0, 0));
	m_bankA->setPosition(A_CONTROLLER_W + A_CONTROLLER_X + 10, A_CONTROLLER_Y);
	m_bankA->setScale(0.7);
	m_skillbuttonA->setPosition(B_CONTROLLER_X + B_CONTROLLER_W / 2, B_CONTROLLER_Y / 2);
	m_skillbuttonA->setScale(0.8);
	m_playerVDA->setScale(_contentSize.height / m_playerVDA->getContentSize().height);
	m_playerVDA->setAnchorPoint(Point(1, 1));
	m_playerVDA->setPosition(0, _contentSize.height);
}
void BattleLayer_2P::addPlayerB(Player* b)
{
	m_playerB = b;
	addChild(m_controllerB = m_playerB->getChessBoard(), 3);
	m_controllerB->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_hpbarB = m_playerB->getHealthBar(), 5);
	m_hpbarB->HealthZeroEvent += std::bind(&BattleLayer_2P::HealthZeroEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_playerIconB = m_playerB->getIcon(), 10);
	addChild(m_healworkerB = m_playerB->getHealWorker(), 11);
	addChild(m_skillbuttonB = m_playerB->getSkillButton(), 5);
	addChild(m_playerVDB = BattleVerticalDraw::create(m_playerB->getShipType()), 95); 
	m_skillbuttonB->SkillExcuteEvent += std::bind(&BattleLayer_2P::SkillExcuteEventHandle, this, std::placeholders::_1, std::placeholders::_2);


	m_hpbarB->setAnchorPoint(Point(0, 1));
	m_hpbarB->setScaleX(-1);
	m_hpbarB->getInfoLabel()->setAnchorPoint(Point(0, 0.5));
	m_hpbarB->getInfoLabel()->setScaleX(-1);
	m_playerIconB->setAnchorPoint(Point(1, 1));
	m_healworkerB->setAnchorPoint(Point(0, 0));
	m_healworkerB->setScaleX(-1);
	m_healworkerB->setOriginPosion(_contentSize.width + m_healworkerA->getContentSize().width, convertToNodeSpace(m_hpbarA->convertToWorldSpace(m_hpbarA->healthToPoint(0))).y, true);
	m_controllerB->setContentSize(Size(B_CONTROLLER_W, B_CONTROLLER_H));
	m_controllerB->setAnchorPoint(Point(0, 0));
	m_controllerB->refreshBackGround();
	m_skillbuttonB->setAnchorPoint(Point(0, 0.5));
	m_skillbuttonB->setScale(0.8);
	m_playerVDB->setScale(_contentSize.height / m_playerVDB->getContentSize().height);
	m_playerVDB->setAnchorPoint(Point(0, 1));


	m_hpbarB->setPosition(B_HPBAR_X+STAGECHANGEOFFSET,B_HPBAR_Y);
	m_playerIconB->setPosition(B_ICON_X + STAGECHANGEOFFSET, B_ICON_Y);
	m_controllerB->setPosition(B_CONTROLLER_X + STAGECHANGEOFFSET, B_CONTROLLER_Y);
	m_skillbuttonB->setPosition(B_SKILLBUTTON_X + STAGECHANGEOFFSET, B_SKILLBUTTON_Y);
	m_playerVDB->setPosition(B_VD_X,B_VD_Y);
}
void BattleLayer_2P::removePlayerB()
{
	if (m_playerB != 0)
	{
		m_controllerB->removeFromParent();
		m_hpbarB->removeFromParent();
		m_bankB->removeFromParent();
		m_healworkerB->removeFromParent();
		m_skillbuttonB->removeFromParent();
		m_playerIconB->removeFromParent();
		m_playerVDB->removeFromParent();
		delete m_playerB;
		m_playerB = 0;
	}
}
void BattleLayer_2P::PlayerAttack(Player* attacker, Player* target, std::vector<Block*>& blocks)
{
	Sprite* temp;
	int count = blocks.size(), totaldamage = 0;;
	auto hpbar = target->getHealthBar();
	auto chessboard = attacker->getChessBoard();
	Point targetP = convertToNodeSpace(hpbar->convertToWorldSpace(hpbar->healthToPoint())),P;
	float flywidth = SpriteFrameCache::getInstance()->getSpriteFrameByName("Shell_S_Fly.png")->getOriginalSize().width;
	float angle;
	for (int i = 0; i < count; i++)
	{
		temp = Sprite::createWithSpriteFrameName("Shell_S.png");
		addChild(temp, 15);
		P = convertToNodeSpace(chessboard->convertToWorldSpace(blocks[i]->getPosition()));
		angle = atan2(targetP.y - P.y, targetP.x - P.x);
		temp->setPosition(P);
		temp->setScale(0);
		temp->setOpacity(200);
		totaldamage += blocks[i]->getMode() * 3;
		temp->runAction(Sequence::create(ScaleTo::create(0.3, 2.5), CallFunc::create([temp, angle]
		{
			temp->setSpriteFrame("Shell_S_Fly.png"); 
			temp->setRotation(-180 * angle / M_PI);			
		}), EaseSineIn::create(MoveTo::create(0.4, Point(targetP.x - flywidth*cos(angle), targetP.y - flywidth*sin(angle)))), CallFunc::create([temp]
		{
			temp->removeFromParent(); 
		}), NULL));
	}
	if (totaldamage > 0)
	{
		if (attacker->attacking++ == 0)
			attacker->getIcon()->runAction(MoveBy::create(0.2, Point(8 * attacker->getHealthBar()->getScaleX(), 0)));
		temp = Sprite::createWithSpriteFrameName("attackA_1.png");
		temp->setScale(0.7);
		addChild(temp, 15);
		temp->setPosition(targetP);
		temp->setVisible(false);
		temp->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([temp, target, totaldamage, this]
		{
			temp->setVisible(true);
			int width = target->getChessBoard()->getContentSize().width*0.05;
			target->getChessBoard()->runAction(Sequence::create(MoveBy::create(0.03, Point(width, 0)), MoveBy::create(0.06, Point(-2 * width, 0)), MoveBy::create(0.05, Point(width + width * 3 / 4, 0)), MoveBy::create(0.045, Point(-width * 3 / 4 * 2, 0)), MoveBy::create(0.04, Point(width * 3 / 4 + width / 2, 0)), MoveBy::create(0.03, Point(-width / 2 * 2, 0)), EaseBounceOut::create(MoveBy::create(0.015, Point(width / 2, 0))), NULL));
			if (!target->healthlocked)
			{
				target->getHealthBar()->decreaseHealth(totaldamage);
			}
		}), Animate::create(AnimationCache::getInstance()->getAnimation("attack")), CallFunc::create([temp, attacker]
		{
			temp->removeFromParent();
			if (--attacker->attacking == 0)
				attacker->getIcon()->runAction(MoveBy::create(0.2, Point(-8 * attacker->getHealthBar()->getScaleX(), 0)));
		}), NULL));
	}
}
void BattleLayer_2P::PlayerLock(Player* locker, Player* target, std::vector<Block*>& blocks)
{
	Sprite* temp;
	auto chessboard = locker->getChessBoard();
	auto targetchessboard = target->getChessBoard();
	int i, j, max = targetchessboard->getTotalBlockCount(), count = blocks.size(), tempcount = count;
	std::vector<int> targetblocks;
	for (i = 0; i < max; i++)
	{
		if (!targetchessboard->isNULL(i) && !targetchessboard->isLocked(i) && targetchessboard->Blocks[i]->canLock)
			targetblocks.push_back(i);
	}
	int total = targetblocks.size(), index, locktime;
	srand(time(0));
	Point P,targetP;
	float angle,sc,flywidth = SpriteFrameCache::getInstance()->getSpriteFrameByName("Lock_S_Fly.png")->getOriginalSize().width;
	for (i = 0,j=0; i < total; i++)
	{
		if (random(0, total - i - 1) < tempcount)
		{
			index = targetblocks[i];
			temp = Sprite::createWithSpriteFrameName("Lock_S.png");
			addChild(temp, 15);
			P = convertToNodeSpace(chessboard->convertToWorldSpace(blocks[j]->getPosition()));
			temp->setPosition(P);
			temp->setScale(0);
			temp->setOpacity(200);
			targetP = convertToNodeSpace(targetchessboard->convertToWorldSpace(targetchessboard->indexToClient(index)));
			angle = atan2(targetP.y - P.y, targetP.x - P.x);
			locktime = blocks[j]->getMode()+3;
			temp->runAction(Sequence::create(ScaleTo::create(0.3, 2.5), CallFunc::create([temp, angle]
			{
				temp->setSpriteFrame("Lock_S_Fly.png");
				temp->setRotation(-180 * angle / M_PI);
			}), MoveTo::create(0.3, Point(targetP.x - flywidth*cos(angle), targetP.y - flywidth*sin(angle))), CallFunc::create([temp, targetchessboard, index, locktime]
			{
				temp->removeFromParent(); 
				if (!targetchessboard->isNULL(index) && !targetchessboard->isLocked(index) && targetchessboard->getBlockType(index) != BlockType::Key)
					targetchessboard->lockBlock(index, locktime);
			}), NULL));
			temp = Sprite::createWithSpriteFrameName("Lock_S_Explosion.png");
			addChild(temp, 15);
			temp->setPosition(targetP);
			temp->setScale(0);
			temp->setOpacity(200);
			sc = targetchessboard->getBlockSize().width*1.4/ temp->getContentSize().width;
			temp->runAction(Sequence::create(DelayTime::create(0.6), EaseSineOut::create(ScaleTo::create(0.15, sc)), CallFunc::create([temp]
			{
				temp->removeFromParent();
			}), NULL));
			tempcount--;
			j++;
		}
		if (tempcount == 0)
			break;
	}
}
void BattleLayer_2P::PlayerGetCoin(Player* target, std::vector<Block*>& blocks)
{
	int i, count = blocks.size(), totalcoin = 0;
	Sprite* temp;
	auto bank = target->getCoinBank();
	auto targetchessboard = target->getChessBoard();
	Point targetP = Point(bank->getPositionX() + bank->getScaleX()*bank->getContentSize().width / 2, bank->getPositionY() + bank->getScaleY()* bank->getContentSize().height * 3 / 4), P;
	float blockw = targetchessboard->getBlockSize().width;
	float blockh = targetchessboard->getBlockSize().height;
	float sw, sh;
	for (i = 0; i < count; i++)
	{
		temp = Sprite::createWithSpriteFrameName("Coin.png");
		addChild(temp, 15);
		P = convertToNodeSpace(targetchessboard->convertToWorldSpace(blocks[i]->getPosition()));
		totalcoin += blocks[i]->getMode();
		temp->setPosition(P);
		temp->setScaleX(sw = blockw / temp->getContentSize().width);
		temp->setScaleY(sh = blockh / temp->getContentSize().height);
		temp->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.3, -0.6*sw, 0.6*sh, 1), ScaleTo::create(0.3, 0.6*sw, 0.6*sh, 1), NULL)));
		temp->runAction(Sequence::create(MoveBy::create(0.3, Point(blockw / 4, -blockh / 2)), MoveTo::create(0.4, targetP), CallFunc::create([temp]{temp->removeFromParent(); }), NULL));
	}
	if (totalcoin>0)
		bank->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create(std::bind(&CoinBank::AddCoin, bank, totalcoin)), NULL));
}
void BattleLayer_2P::PlayerGetMana(Player* manar, Player* target, std::vector<Block*>& blocks)
{
	int i, count = blocks.size(), totalmana = 0;
	auto chessboard = manar->getChessBoard();
	auto* manabar = target->getSkillButton();
	Point targetP = convertToNodeSpace(manabar->convertToWorldSpace(manabar->manaToPoint())), P;
	Sprite* temp;
	float sc, tasc,blockw = chessboard->getBlockSize().width,blockh = chessboard->getBlockSize().height;
	for (i = 0; i < count; i++)
	{
		temp = Sprite::createWithSpriteFrameName("colorfulstar.png");
		addChild(temp, 15);
		P = convertToNodeSpace(chessboard->convertToWorldSpace(blocks[i]->getPosition()));
		totalmana += blocks[i]->getMode() * 5;
		temp->setPosition(P);
		temp->setScale(0);
		sc = blockh / temp->getContentSize().height*0.8;
		tasc = manabar->getStripWidth()*manabar->getScaleX()/temp->getContentSize().width;
		temp->runAction(RepeatForever::create(RotateBy::create(0.7, 360)));
		temp->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.3, sc), MoveBy::create(0.3, Point(blockw / 4, -blockh / 2)), NULL), Spawn::create(EaseSineIn::create(MoveTo::create(0.4, targetP)), ScaleTo::create(0.4, tasc), NULL), CallFunc::create([temp]{temp->removeFromParent(); }), NULL));
	}
	if (totalmana>0)
		manabar->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([manabar, totalmana]{manabar->increaseMana(totalmana); }), NULL));
}
void BattleLayer_2P::PlayerHeal(Player* healer, Player* target, std::vector<Block*>& blocks)
{
	int i, j, count = blocks.size(),totalheal = 0;
	Sprite* temp;
	auto targetchessboard = healer->getChessBoard();
	Point P;
	float blockw = targetchessboard->getBlockSize().width, blockh = targetchessboard->getBlockSize().height,s,p;
	ccBezierConfig config;
	auto hpbar = target->getHealthBar();
	for (i = 0; i < count; i++)
	{
		p = 0.2;
		s = 2.5;
		totalheal += blocks[i]->getMode();
		for (j = 0; j < 4; j++)
		{
			if (j % 3 == 0)
			{
				temp = Sprite::createWithSpriteFrameName("health+G.png");
			}
			else
			{
				temp = Sprite::createWithSpriteFrameName("health+R.png");
			}
			addChild(temp, 15-j);
			P = convertToNodeSpace(targetchessboard->convertToWorldSpace(blocks[i]->getPosition()));
			temp->setPosition(P);
			temp->setScale(blockw*0.1 / temp->getContentSize().width);
			config.controlPoint_1 = Point(blockw *p , 0);
			config.endPosition = Point(blockw *p, blockh);
			config.controlPoint_2 = config.endPosition;
			temp->runAction(Sequence::create(DelayTime::create(j*0.1), Spawn::create(EaseSineOut::create(ScaleBy::create(0.8, s)), BezierBy::create(0.8, config), EaseSineIn::create(FadeOut::create(0.8 - j*0.1)), NULL), CallFunc::create([temp]{temp->removeFromParent(); }), NULL));
			p *= -0.75;
			s *= 0.9;
		}
	}
	if (totalheal>0&&!target->healthlocked&&hpbar->increaseHealth(totalheal))
	{
		target->getHealWorker()->work(convertToNodeSpace(hpbar->convertToWorldSpace(hpbar->healthToPoint())));
	}
}
void BattleLayer_2P::BlockClearedEventHandle(ChessBoard* sender, BlockClearEventArg* arg)
{
	int i, count = arg->blocks.size();
	std::vector<Block*> attackblocks;
	std::vector<Block*> coinblocks;
	std::vector<Block*> lockblocks;
	std::vector<Block*> manablocks;
	std::vector<Block*> recoverblocks;
	Player* launcher;
	Player* target;
	if (sender == m_controllerA)
	{
		launcher = m_playerA;
		target = m_playerB;
	}
	else
	{
		launcher = m_playerB;
		target = m_playerA;
	}
	Block* tempB;
	for (i = 0; i < count; i++)
	{
		tempB = arg->blocks[i];
		switch (tempB->type)
		{
		case BlockType::Attack:
			attackblocks.push_back(tempB);
			break;
		case BlockType::Coin:
			coinblocks.push_back(tempB);
			break;
		case BlockType::Lock:
			lockblocks.push_back(tempB);
			break;
		case BlockType::Mana:
			manablocks.push_back(tempB);
			break;
		case BlockType::Recover:
			recoverblocks.push_back(tempB);
			break;
		default:
			break;
		}
	}
	if (attackblocks.size() > 0)
		PlayerAttack(launcher, target, attackblocks);
	if (launcher==m_playerA&&coinblocks.size() > 0)
		PlayerGetCoin(launcher,coinblocks);
	if (lockblocks.size() > 0)
		PlayerLock(launcher, target, lockblocks);
	if (manablocks.size() > 0)
		PlayerGetMana(launcher, launcher, manablocks);
	if (recoverblocks.size() > 0)
		PlayerHeal(launcher, launcher, recoverblocks);
}
void BattleLayer_2P::HealthZeroEventHandle(HealthBar* sender, EventArg* arg)
{
	Player* loser;
	Player* winner;
	if (sender == m_hpbarA)
	{
		winner = m_playerB;
		loser = m_playerA;
	}
	else
	{
		winner = m_playerA;
		loser = m_playerB;
	}
	_eventDispatcher->pauseEventListenersForTarget(this);
	winner->gameStop();
	loser->gameStop();
	runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([winner, loser]{
		auto losercontroller = loser->getChessBoard();
		auto losericon = loser->getIcon();
		int width = losercontroller->getContentSize().width*0.05, i, j;
		cocos2d::Vector<FiniteTimeAction*> actions;
		losericon->setIconType(IconType::Sinked);
		float speed = 0.03;
		for (i = 0; i < 15; i++)
		{
			actions.pushBack(MoveBy::create(speed, Point(width, 0)));
			actions.pushBack(MoveBy::create(2 * speed, Point(-2 * width, 0)));
			actions.pushBack(MoveBy::create(speed, Point(width, 0)));
			if (i % 2 == 1)
			{
				width = width * 9 / 10;
				speed = speed * 9 / 10;
			}
		}
		actions.pushBack(DelayTime::create(0.6));
		actions.pushBack(CallFunc::create([losercontroller]{losercontroller->destroyAll(1); }));
		losercontroller->runAction(Sequence::create(actions));
		Sprite* tempS;
		float iconwidth = losericon->getContentSize().width;
		float iconheight = losericon->getContentSize().height;
		int eachexplosioncount = 3;
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < eachexplosioncount; j++)
			{
				losericon->addChild(tempS = Sprite::createWithSpriteFrameName("attackA_1.png"));
				tempS->setScale(0.7);
				tempS->setVisible(false);
				tempS->setPosition(rand_0_1()*iconwidth / eachexplosioncount + iconwidth * j / eachexplosioncount, rand_0_1()*iconheight);
				tempS->runAction(Sequence::create(DelayTime::create(0.3*(i*eachexplosioncount + j)), CallFunc::create([tempS]{tempS->setVisible(true); }), Animate::create(AnimationCache::getInstance()->getAnimation("attack")), CallFunc::create([tempS]{tempS->removeFromParent(); }), NULL));
			}
		}
	}), DelayTime::create(5), CallFunc::create([this]
	{
		Director::getInstance()->popScene();
	}), NULL));
}
void BattleLayer_2P::SkillExcuteEventHandle(SkillButton* sender, SkillExcuteEventArg* arg)
{
	Player* skiller;
	Player* target;
	BattleVerticalDraw* skillervd;
	if (sender == m_skillbuttonA)
	{
		skiller = m_playerA;
		target = m_playerB;
		skillervd = m_playerVDA;
		m_playerVDA->stopAllActions();
		m_playerVDA->runAction(Sequence::create(MoveTo::create(0.2, Point(m_playerVDA->getContentSize().width*m_playerVDA->getScale(), _contentSize.height)), DelayTime::create(0.2), CallFunc::create([skillervd]{skillervd->splash(0.25); }), DelayTime::create(0.4), MoveTo::create(0.2, Point(0, _contentSize.height)), NULL));
	}
	else
	{
		skiller = m_playerB;
		target = m_playerA; 
		skillervd = m_playerVDB;
		m_playerVDB->stopAllActions();
		m_playerVDB->runAction(Sequence::create(MoveTo::create(0.2, Point(_contentSize.width - m_playerVDB->getContentSize().width*m_playerVDB->getScale(), _contentSize.height)), DelayTime::create(0.2), CallFunc::create([skillervd]{skillervd->splash(0.25); }), DelayTime::create(0.4), MoveTo::create(0.2, Point(_contentSize.width, _contentSize.height)), NULL));
	}
	skiller->getSkills()[arg->index]->excute(skiller, target, this);
}
bool BattleLayer_2P::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	m_controllerA->onTouchBegan(m_controllerA->convertTouchToNodeSpace(touch));
	m_skillbuttonA->onTouchBegan(m_skillbuttonA->convertTouchToNodeSpace(touch));
	return true;
}
void BattleLayer_2P::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	m_controllerA->onTouchMoved(m_controllerA->convertTouchToNodeSpace(touch));
	m_skillbuttonA->onTouchMoved(m_skillbuttonA->convertTouchToNodeSpace(touch));
}

void BattleLayer_2P::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	m_skillbuttonA->onTouchEnded(m_skillbuttonA->convertTouchToNodeSpace(touch));
}
void BattleLayer_2P::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	m_skillbuttonA->onTouchCancelled(m_skillbuttonA->convertTouchToNodeSpace(touch));
}