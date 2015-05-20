#include "Player.h"
#include "Block.h"
#include "ChessBoard.h"
#include "HealthBar.h"
#include "CoinBank.h"
#include "HealWorker.h"
#include "SkillButton.h"
#include "Icon.h"
#include "cocos2d.h"
#include "Ship.h"

using namespace cocos2d;

Player::Player(Ship* c) :m_ship(c), m_controller(0), m_hpbar(0), m_icon(0), m_healworker(0), m_skillbutton(0), m_coinbank(0), attacking(0), healthlocked(false)
{
}
Player::~Player()
{

}
void Player::gameStart()
{
	healthlocked = false;
	if (m_hpbar->getCurrentHealth() == -1)
	{
		m_hpbar->updateHealth(m_ship->Hp);
		m_controller->allowHelp(3);
		m_controller->start();
	}
	else if (m_controller != NULL)
	{
		m_controller->scheduleUpdate();
	}

}

void Player::gameStop()
{
	healthlocked = true;
	if (m_controller != NULL)
	{
		m_controller->unscheduleAllCallbacks();
	}
}

HealthBar* Player::getHealthBar()
{
	return m_hpbar == NULL ? m_hpbar = HealthBar::create(m_ship->Hp, CCString::createWithFormat("%s  LV %d", m_ship->shipdisplayname.c_str(), m_ship->Level)->_string) : m_hpbar;
}
ChessBoard* Player::getChessBoard()
{
	return m_controller == NULL ? m_controller = ChessBoard::create(6, 6) : m_controller;
}
Icon* Player::getIcon()
{
	return m_icon == NULL ? m_icon = Icon::create(m_ship->shiptype) : m_icon;
}
HealWorker* Player::getHealWorker()
{
	return m_healworker == NULL ? m_healworker = HealWorker::create() : m_healworker;
}
SkillButton* Player::getSkillButton()
{
	return m_skillbutton == NULL ? m_skillbutton = SkillButton::create(m_ship->Skills) : m_skillbutton;
}
CoinBank* Player::getCoinBank()
{
	return m_coinbank == NULL ? m_coinbank = CoinBank::create() : m_coinbank;
}

SkillButton* Player_AI::getSkillButton()
{
	return m_skillbutton == NULL ? m_skillbutton = SkillButtonSimple::create(m_ship->Skills) : m_skillbutton;
}
void Player_AI::gameStart()
{
	healthlocked = false;
	srand(time(NULL));
	if (m_controller->isScheduled("moveai0"))
		m_controller->unschedule("moveai0");
	if (m_controller->isScheduled("moveai1"))
		m_controller->unschedule("moveai1");
	float time = m_hardgrade*(1 + random(-0.2, 0.2));
	m_currentschedule = (m_currentschedule + 1) % 2;
	m_controller->scheduleOnce(std::bind(&Player_AI::move, this, std::placeholders::_1), time < 1 ? 1 : time, "moveai" + m_currentschedule);
	if (m_hpbar->getCurrentHealth() == -1)
	{
		m_hpbar->updateHealth(m_ship->Hp);
		m_controller->start();
	}
	else
	{
		m_controller->resume();
	}
}
void Player_AI::move(float dt)
{
	int x, y, index_1, index_2, max_1 = -1, max_2 = -1, maxscore = -1, temp, lockcount = 0, keyindex = -1;
	int cfs[5] = { 2, 2, 2, 2, 0 };
	if (getHealthBar()->getCurrentHealth() == m_ship->Hp)
		cfs[BlockType::Recover] = 0;
	Block* tempB;
	for (x = 0; x < 36; x++)
	{
		tempB = m_controller->Blocks[x];
		if (tempB != NULL)
		{
			if (tempB->isLocked())
				lockcount++;
			if (tempB->type == BlockType::Key&&tempB->animeCount == 0)
				keyindex = x;
		}
	}
	if (lockcount > 0 && keyindex != -1)
	{
		m_controller->pressBlock(keyindex);

		float time = m_hardgrade*(1 + random(-0.2, 0.2));
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
	float time = m_hardgrade*(1 + random(-0.2, 0.2));
	m_currentschedule = (m_currentschedule + 1) % 2;
	m_controller->scheduleOnce(std::bind(&Player_AI::move, this, std::placeholders::_1), time<1 ? 1 : time, "moveai" + m_currentschedule);
}
Player_AI::~Player_AI()
{
	delete m_ship;
}