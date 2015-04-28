#include "GameLayer.h"
#include "Block.h"
#include "Characters.h"
#include "ChessBoard.h"
#include "HealthBar.h"
#include <cmath>

using namespace cocos2d;

Player::Player(Character* c) :m_character(c), Character(*c), m_controller(0), m_hpbar(0), m_icon(0), attacking(0)
{
	currentHp = HP;
	currentMana = Mana;
}
void Player::gameStart()
{
	m_controller->allowTouch();
	m_controller->allowHelp(3);
	m_hpbar->updateHealth(HP);
	m_controller->start();
}
inline HealthBar* Player::getHealthBar()
{
	return m_hpbar == NULL ? m_hpbar = HealthBar::create(HP):m_hpbar;
}
inline ChessBoardLayer* Player::getChessBoard()
{
	return m_controller == NULL ? m_controller = ChessBoardLayer::create(6, 6) : m_controller;
}
inline Sprite* Player::getIcon()
{
	if (m_icon == NULL)
	{
		Sprite* temp;
		m_icon = Sprite::createWithSpriteFrameName("characterflame.png");
		m_icon->addChild(temp = Sprite::createWithSpriteFrameName(m_character->Name + "BattleIcon.png"));
		temp->setPosition(m_icon->getContentSize() / 2);
	}
	return m_icon;
}
void Player_AI::gameStart()
{
	srand(time(NULL));
	m_hpbar->updateHealth(HP);
	if (m_controller->isScheduled("moveai0"))
		m_controller->unschedule("moveai0");	
	if (m_controller->isScheduled("moveai1"))
		m_controller->unschedule("moveai1");
	float time = HardGrade*(1 + random(-0.2, 0.2));
	m_currentschedule = (m_currentschedule + 1) % 2;
	m_controller->scheduleOnce(std::bind(&Player_AI::move, this, std::placeholders::_1), time<1 ? 1 : time, "moveai"+m_currentschedule);
	m_controller->start();
}
void Player_AI::move(float dt)
{
	int x, y, index_1, index_2, max_1 = -1, max_2 = -1, maxscore = -1, temp,lockcount=0,keyindex = -1;
	int cfs[5] = { 2, 2, 2, 2, 0 };
	if (currentHp == HP)
		cfs[BlockType::Recover] = 0;
	Block* tempB;
	for (x = 0; x < 36; x++)
	{
		tempB = m_controller->m_blocks[x];
		if (tempB != NULL)
		{
			if (tempB->isLocked())
				lockcount++;
			if (tempB->type == BlockType::Key&&tempB->animeCount==0)
				keyindex = x;
		}
	}
	if (lockcount > 0 && keyindex != -1)
	{
		m_controller->pressBlock(keyindex);

		float time = HardGrade*(1 + random(-0.2, 0.2));
		m_currentschedule = (m_currentschedule + 1) % 2;
		m_controller->scheduleOnce(std::bind(&Player_AI::move, this, std::placeholders::_1), time<1 ? 1 : time, "moveai" + m_currentschedule);
		return;
	}
	for (y = 0; y < 5; y++)
	{
		index_1 = m_controller->pointToIndex(0, y);
		for (x = 0; x < 5; x++, index_1++)
		{
			if (!m_controller->isLocked(index_1))
			{
				if (!m_controller->isLocked(index_2 = index_1 + 1))
				{
					temp = m_controller->checkSwap(index_1, index_2, cfs);
					if (temp>maxscore)
					{
						maxscore = temp;
						max_1 = index_1;
						max_2 = index_2;
					}
				}
				if (!m_controller->isLocked(index_2 = index_1 + 6))
				{
					temp = m_controller->checkSwap(index_1, index_2, cfs);
					if (temp > maxscore)
					{
						maxscore = temp;
						max_1 = index_1;
						max_2 = index_2;
					}
				}
			}
		}
		if (!m_controller->isLocked(index_1) && !m_controller->isLocked(index_2 = index_1 + 6))
		{
			temp = m_controller->checkSwap(index_1, index_2, cfs);
			if (temp > maxscore)
			{
				maxscore = temp;
				max_1 = index_1;
				max_2 = index_2;
			}
		}
	}
	index_1 = m_controller->pointToIndex(0, y);
	for (x = 0; x < 5; x++, index_1++)
	{
		if (!m_controller->isLocked(index_1) && !m_controller->isLocked(index_2 = index_1 + 1))
		{
			temp = m_controller->checkSwap(index_1, index_2, cfs);
			if (temp>maxscore)
			{
				maxscore = temp;
				max_1 = index_1;
				max_2 = index_2;
			}
		}
	}
	if (max_1 >= 0)
	{
		m_controller->trySwap(max_1, max_2);
	}
	float time = HardGrade*(1 + random(-0.2, 0.2));
	m_currentschedule = (m_currentschedule + 1) % 2;
	m_controller->scheduleOnce(std::bind(&Player_AI::move, this, std::placeholders::_1), time<1 ? 1 : time, "moveai" + m_currentschedule);
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battleicon.plist", "battleicon.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("attackanime.plist", "attackanime.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background.plist", "background.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spritesheet.plist", "spritesheet.png");
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
	addChild(m_controllerB = m_playerB->getChessBoard(),3);
	addChild(m_hpbarA = m_playerA->getHealthBar(),5);
	addChild(m_hpbarB = m_playerB->getHealthBar(),5);
	addChild(m_playerIconA = m_playerA->getIcon(), 10);
	addChild(m_playerIconB = m_playerB->getIcon(), 10, 10);


	addChild(m_background = Sprite::createWithSpriteFrameName("battlemain.png"),0);
	addChild(m_VS = Sprite::createWithSpriteFrameName("VS.png"), 10);
	addChild(m_VSframe = Sprite::createWithSpriteFrameName("Gear.png"), 9);
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

	m_playerIconA->setAnchorPoint(Point(0, 1));
	m_playerIconA->setPosition(0, _contentSize.height);
	m_playerIconB->setAnchorPoint(Point(1, 1));
	m_playerIconB->setPosition(_contentSize.width, _contentSize.height);

	m_VS->setPosition(_contentSize.width / 2, _contentSize.height- m_hpbarA->getContentSize().height / 2);
	m_VSframe->setPosition(m_VS->getPosition());
	if (m_VSframe->getNumberOfRunningActions()==0)
		m_VSframe->runAction(RepeatForever::create(RotateBy::create(36, 360)));

	m_controllerA->setContentSize(Size(420, 372));
	m_controllerB->setContentSize(Size(246, 218));
	m_controllerA->setPosition(23, 8);
	m_controllerB->setPosition(506, 161);
	m_controllerA->refreshBackGround();
	m_controllerB->refreshBackGround();
	m_controllerA->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
	m_controllerB->blockClearEvent += std::bind(&BattleLayer_2P::BlockClearedEventHandle, this, std::placeholders::_1, std::placeholders::_2);
}
void BattleLayer::AttackPlayer(Player* target, int damage)
{
	int result = target->currentHp - damage;
	target->currentHp = result >= 0 ? result : 0;
	target->m_hpbar->updateHealth(target->currentHp);
}
void BattleLayer::HealPlayer(Player* target, int number)
{
	int result = target->currentHp + number;
	target->currentHp = result > target->HP ? target->HP : result;
	target->m_hpbar->updateHealth(target->currentHp);
}
void BattleLayer::LockPlayer(Player* target, int number)
{
	int i,k,count = target->m_controller->getTotalBlockCount();
	std::vector<int> blocks;
	for (i = 0; i < count; i++)
	{
		if (!target->m_controller->isNULL(i)&&!target->m_controller->isLocked(i))
			blocks.push_back(i);
	}
	int total = blocks.size();
	srand(time(0));
	k = number;
	for (i = 0; i < total; i++)
	{
		if (random(0, total - i - 1) < k)
		{
			target->m_controller->lockBlock(blocks[i],4);
			k--;
		}
	}
}
void BattleLayer_2P::PlayerAttack(Player* attacker, Player* target, BlockClearEventArg* arg)
{
	if (attacker->attacking++ == 0)
		attacker->getIcon()->runAction(MoveBy::create(0.2, Point(8 * attacker->getHealthBar()->getScaleX(), 0)));
	Sprite* temp;
	int count = arg->locations.size();
	auto hpbar = target->getHealthBar();
	auto chessboard = attacker->getChessBoard();
	Point P;
	Point targetP = convertToNodeSpace(hpbar->convertToWorldSpace(hpbar->healthToPoint(hpbar->getCurrentHealth())));
	float flywidth = SpriteFrameCache::getInstance()->getSpriteFrameByName("Shell_S_Fly.png")->getOriginalSize().width;
	float angle;
	for (int i = 0; i < count; i++)
	{
		temp = Sprite::createWithSpriteFrameName("Shell_S.png");
		addChild(temp, 15);
		P = convertToNodeSpace(chessboard->convertToWorldSpace(arg->locations[i]));
		angle = atan2(targetP.y - P.y, targetP.x - P.x);
		temp->setPosition(P);
		temp->setScale(0);
		temp->setOpacity(200);
		temp->runAction(Sequence::create(ScaleTo::create(0.3, 2.5), CallFunc::create([temp, angle]
		{
			temp->setSpriteFrame("Shell_S_Fly.png"); 
			temp->setRotation(-180 * angle / M_PI);			
		}), EaseSineIn::create(MoveTo::create(0.4, Point(targetP.x - flywidth*cos(angle), targetP.y - flywidth*sin(angle)))), CallFunc::create([temp]
		{
			temp->removeFromParent(); 
		}), NULL));
	}
	temp = Sprite::createWithSpriteFrameName("attackA_1.png");
	temp->setScale(0.7);
	addChild(temp, 15);
	temp->setPosition(targetP);
	temp->setVisible(false);
	int damage = arg->totalcount*3;
	temp->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([temp, target, damage]
	{
		temp->setVisible(true);
		int result = target->currentHp - damage;
		target->currentHp = result >= 0 ? result : 0;
		target->getHealthBar()->updateHealth(target->currentHp);
		int width = target->getChessBoard()->getContentSize().width*0.05;
		target->getChessBoard()->runAction(Sequence::create(MoveBy::create(0.03, Point(width, 0)), MoveBy::create(0.06, Point(-2*width, 0)), MoveBy::create(0.05, Point(width + width*3/4, 0)), MoveBy::create(0.045, Point(-width*3/4*2, 0)), MoveBy::create(0.04, Point(width*3/4+width/2, 0)), MoveBy::create(0.03, Point(-width/2*2, 0)), EaseBounceOut::create(MoveBy::create(0.015, Point(width/2, 0))), NULL));
	}), Animate::create(AnimationCache::getInstance()->getAnimation("attack")), CallFunc::create([temp, attacker]
	{
		temp->removeFromParent(); 
		if (--attacker->attacking == 0)
			attacker->getIcon()->runAction(MoveBy::create(0.2, Point(-8 * attacker->getHealthBar()->getScaleX(), 0)));
	}), NULL));
}
void BattleLayer_2P::PlayerLock(Player* locker, Player* target, BlockClearEventArg* arg)
{
	Sprite* temp;
	int count = arg->locations.size(),tempcount = count;
	auto hpbar = target->getHealthBar();
	auto chessboard = locker->getChessBoard();
	auto targetchessboard = target->getChessBoard();
	int i, j, max = targetchessboard->getTotalBlockCount();
	std::vector<int> blocks;
	for (i = 0; i < max; i++)
	{
		if (!targetchessboard->isNULL(i) && !targetchessboard->isLocked(i) && targetchessboard->getBlockType(i)!=BlockType::Key)
			blocks.push_back(i);
	}
	int total = blocks.size(),index;
	srand(time(0));
	Point P;
	Point targetP;
	int mode;
	float angle;
	float flywidth = SpriteFrameCache::getInstance()->getSpriteFrameByName("Lock_S_Fly.png")->getOriginalSize().width;
	float sc;
	for (i = 0,j=0; i < total; i++)
	{
		if (random(0, total - i - 1) < tempcount)
		{
			index = blocks[i];
			temp = Sprite::createWithSpriteFrameName("Lock_S.png");
			addChild(temp, 15);
			P = convertToNodeSpace(chessboard->convertToWorldSpace(arg->locations[j]));
			temp->setPosition(P);
			temp->setScale(0);
			temp->setOpacity(200);
			targetP = convertToNodeSpace(targetchessboard->convertToWorldSpace(targetchessboard->indexToClient(index)));
			angle = atan2(targetP.y - P.y, targetP.x - P.x);
		    mode = arg->modes[j];
			temp->runAction(Sequence::create(ScaleTo::create(0.3, 2.5), CallFunc::create([temp, angle]
			{
				temp->setSpriteFrame("Lock_S_Fly.png");
				temp->setRotation(-180 * angle / M_PI);
			}), MoveTo::create(0.3, Point(targetP.x - flywidth*cos(angle), targetP.y - flywidth*sin(angle))), CallFunc::create([temp, targetchessboard, index, mode]
			{
				temp->removeFromParent(); 
				if (!targetchessboard->isNULL(index) && !targetchessboard->isLocked(index) && targetchessboard->getBlockType(index) != BlockType::Key)
					targetchessboard->lockBlock(index, 3 + mode); 
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
void BattleLayer_2P::BlockClearedEventHandle(void* sender, BlockClearEventArg* arg)
{
	switch (arg->type)
	{
	case BlockType::Attack:
		if (sender == m_controllerA)
		{
			PlayerAttack(m_playerA, m_playerB, arg);
		}
		else
		{
			PlayerAttack(m_playerB, m_playerA, arg);
		}
		break;
	case BlockType::Coin:break;
	case BlockType::Lock:
		if (sender == m_controllerA)
		{
			PlayerLock(m_playerA,m_playerB, arg);
		}
		else
		{
			PlayerLock(m_playerB, m_playerA, arg);
		}
		break;
	case BlockType::Mana:break;
	case BlockType::Recover:
		if (sender == m_controllerA)
		{
			HealPlayer(m_playerA, arg->totalcount);
		}
		else
		{
			HealPlayer(m_playerB, arg->totalcount);
		}
		break;
	default:
		break;
	}
}
