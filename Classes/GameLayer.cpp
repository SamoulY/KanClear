#include "GameLayer.h"
#include "Block.h"
#include "ChessBoard.h"
#include "HealthBar.h"
#include "CoinBank.h"
#include "HealWorker.h"
#include "SkillButton.h"
#include "Icon.h"
#include "Player.h"
#include <cmath>

using namespace cocos2d;

void BattleLayer_2P::gameStart()
{
	m_playerA->gameStart();
	m_playerB->gameStart();
}
BattleLayer_2P* BattleLayer_2P::createWithPlayer(Player* A, Player* B)
{
	BattleLayer_2P* gl = new BattleLayer_2P(A,B);
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/battleicon.plist", "Resources/img/battleicon.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/attackanime.plist", "Resources/img/attackanime.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/background.plist", "Resources/img/background.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Resources/img/spritesheet.plist", "Resources/img/spritesheet.png");
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
	

	addChild(m_controllerA = m_playerA->getChessBoard(),3);
	m_controllerA->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_controllerB = m_playerB->getChessBoard(), 3); 
	m_controllerB->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_hpbarA = m_playerA->getHealthBar(),5);
	m_hpbarA->HealthZeroEvent += std::bind(&BattleLayer_2P::HealthZeroEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_hpbarB = m_playerB->getHealthBar(), 5);
	m_hpbarB->HealthZeroEvent += std::bind(&BattleLayer_2P::HealthZeroEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_playerIconA = m_playerA->getIcon(), 10);
	addChild(m_playerIconB = m_playerB->getIcon(), 10);
	addChild(m_healworkerA = m_playerA->getHealWorker(), 11);
	addChild(m_healworkerB = m_playerB->getHealWorker(), 11);
	addChild(m_skillbuttonA = m_playerA->getSkillButton(), 5);
	m_skillbuttonA->SkillExcuteEvent += std::bind(&BattleLayer_2P::SkillExcuteEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	addChild(m_skillbuttonB = m_playerB->getSkillButton(), 5);
	m_skillbuttonB->SkillExcuteEvent += std::bind(&BattleLayer_2P::SkillExcuteEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	
	addChild(m_bankA = m_playerA->getCoinBank(), 20);

	addChild(m_background = Sprite::createWithSpriteFrameName("battlemain.png"),0);
	addChild(m_VS = Sprite::createWithSpriteFrameName("VS.png"), 10);
	addChild(m_VSframe = Sprite::createWithSpriteFrameName("Gear.png"), 9);

	addChild(m_playerVDA = Sprite::createWithSpriteFrameName(String::createWithFormat("%sVD.png", m_playerA->Name.c_str())->_string), 100);
	addChild(m_playerVDB = Sprite::createWithSpriteFrameName(String::createWithFormat("%sVD.png", m_playerB->Name.c_str())->_string), 100);

	ClippingNode* cln;
	Sprite* mask;
	m_playerVDA->setOpacity(230);
	m_playerVDA->addChild(cln = ClippingNode::create());
	cln->setStencil(mask = Sprite::createWithSpriteFrame(m_playerVDA->getSpriteFrame()));
	mask->setAnchorPoint(Point(0, 0));
	cln->setAlphaThreshold(0);
	cln->addChild(m_playerVDblinkA = CCLayerColor::create(Color4B(255, 255, 255, 255), m_playerVDA->getContentSize().width, m_playerVDA->getContentSize().height));
	m_playerVDB->setOpacity(230);
	m_playerVDB->addChild(cln = ClippingNode::create());
	cln->setStencil(mask = Sprite::createWithSpriteFrame(m_playerVDB->getSpriteFrame()));
	mask->setAnchorPoint(Point(0, 0));
	cln->setAlphaThreshold(0);
	cln->addChild(m_playerVDblinkB = CCLayerColor::create(Color4B(255, 255, 255, 255), m_playerVDB->getContentSize().width, m_playerVDB->getContentSize().height));
	
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(BattleLayer_2P::onTouchBegan, this);
	touchlistener->onTouchMoved = CC_CALLBACK_2(BattleLayer_2P::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
	return true;
}
void BattleLayer_2P::initLayout()
{
	m_background->setPosition(_contentSize/2);
	m_hpbarA->setAnchorPoint(Point(0, 1));
	m_hpbarA->setPosition(0, _contentSize.height);
	m_hpbarB->setAnchorPoint(Point(0, 1));
	m_hpbarB->setPosition(_contentSize.width, _contentSize.height);
	m_hpbarB->setScaleX(-1);
	m_hpbarB->getInfoLabel()->setAnchorPoint(Point(0, 0.5));
	m_hpbarB->getInfoLabel()->setScaleX(-1);
	

	m_playerIconA->setAnchorPoint(Point(0, 1));
	m_playerIconA->setPosition(0, _contentSize.height);
	m_playerIconB->setAnchorPoint(Point(1, 1));
	m_playerIconB->setPosition(_contentSize.width, _contentSize.height);

	m_healworkerA->setAnchorPoint(Point(0, 0));
	m_healworkerA->setOriginPosion(-m_healworkerA->getContentSize().width, convertToNodeSpace(m_hpbarA->convertToWorldSpace(m_hpbarA->healthToPoint(0))).y,true);
	m_healworkerB->setAnchorPoint(Point(0, 0));
	m_healworkerB->setScaleX(-1);
	m_healworkerB->setOriginPosion(_contentSize.width + m_healworkerA->getContentSize().width, convertToNodeSpace(m_hpbarA->convertToWorldSpace(m_hpbarA->healthToPoint(0))).y,true);
	

	m_VS->setPosition(_contentSize.width / 2, _contentSize.height- m_hpbarA->getContentSize().height / 2);
	m_VSframe->setPosition(m_VS->getPosition());
	if (m_VSframe->getNumberOfRunningActions()==0)
		m_VSframe->runAction(RepeatForever::create(RotateBy::create(36, 360)));

	int controllerAW = 420, controllerAH = 372, controllerBW = 246, controllerBH = 218,
		controllerAX = 23, controllerAY = 8, controllerBX = 506, controllerBY = 161;
	m_controllerA->setContentSize(Size(controllerAW, controllerAH));
	m_controllerA->setAnchorPoint(Point(0, 0));
	m_controllerB->setContentSize(Size(controllerBW, controllerBH));
	m_controllerB->setAnchorPoint(Point(0, 0));
	m_controllerA->setPosition(controllerAX, controllerAY);
	m_controllerB->setPosition(controllerBX, controllerBY);
	m_controllerA->refreshBackGround();
	m_controllerB->refreshBackGround();

	m_bankA->setAnchorPoint(Point(0, 0));
	m_bankA->setPosition(controllerAW + controllerAX + 10, controllerAY);
	m_bankA->setScale(0.7);

	m_skillbuttonA->setPosition(controllerBX + controllerBW / 2, controllerBY / 2);
	m_skillbuttonA->setScale(0.8);
	m_skillbuttonB->setAnchorPoint(Point(0,0.5));
	m_skillbuttonB->setPosition(controllerBX + controllerBW, controllerBY + controllerBH/2);
	m_skillbuttonB->setScale(0.8);

	m_playerVDA->setScale(_contentSize.height / m_playerVDA->getContentSize().height);
	m_playerVDA->setAnchorPoint(Point(1, 1));
	m_playerVDA->setPosition(0,_contentSize.height);
	m_playerVDB->setScale(_contentSize.height / m_playerVDB->getContentSize().height);
	m_playerVDB->setAnchorPoint(Point(0, 1));
	m_playerVDB->setPosition(_contentSize.width, _contentSize.height);
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
void BattleLayer_2P::HealthZeroEventHandle(HealthBar* sender, HealthZeroEventArg* arg)
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
	winner->gameWin();
	loser->gameOver();
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
	}), NULL));
}
void BattleLayer_2P::SkillExcuteEventHandle(SkillButton* sender, SkillExcuteEventArg* arg)
{
	Player* skiller;
	Player* target;
	Sprite* temp;
	if (sender == m_skillbuttonA)
	{
		skiller = m_playerA;
		target = m_playerB;
		m_playerVDA->stopAllActions();
		m_playerVDblinkA->stopAllActions();
		m_playerVDblinkA->setOpacity(0);
		m_playerVDA->runAction(Sequence::create(MoveTo::create(0.2, Point(m_playerVDA->getContentSize().width*m_playerVDA->getScale(), _contentSize.height)), DelayTime::create(0.6), MoveTo::create(0.2, Point(0, _contentSize.height)), NULL));
		m_playerVDblinkA->runAction(Sequence::create(DelayTime::create(0.4), EaseSineIn::create(FadeIn::create(0.05)), FadeOut::create(0.2), NULL));
	}
	else
	{
		skiller = m_playerB;
		target = m_playerA; 
		m_playerVDB->stopAllActions();
		m_playerVDblinkB->stopAllActions();
		m_playerVDblinkB->setOpacity(0);
		m_playerVDB->runAction(Sequence::create(MoveTo::create(0.2, Point(_contentSize.width - m_playerVDB->getContentSize().width*m_playerVDB->getScale(), _contentSize.height)), DelayTime::create(0.6), MoveTo::create(0.2, Point(_contentSize.width, _contentSize.height)), NULL));
		m_playerVDblinkB->runAction(Sequence::create(DelayTime::create(0.4), EaseSineIn::create(FadeIn::create(0.05)), FadeOut::create(0.2), NULL));
	}
	skiller->Skills[arg->index]->excute(skiller, target, this);
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
}