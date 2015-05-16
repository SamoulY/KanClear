#ifndef _BLOCK_H_
#define _BLOCK_H_


#include "cocos2d.h"

enum BlockType
{
	Attack,
	Lock,
	Recover,
	Mana,
	Coin,
	Count = 5,
	Key
};

class Block :public cocos2d::Sprite
{
public:
	static Block* create(BlockType type);
	BlockType type;
	bool selected;
	int animeCount;
	bool rowChecked;
	bool colChecked;
	bool canLock;
	bool canPress;
	int chaincount;
	int x;
	int y;
	int getMode();
	void setMode(int mode);
	bool isLocked()
	{
		return m_locked;
	}
	void lock(float time);
	void unlock();
protected:
	Block(BlockType type) : m_locked(false), canLock(true),canPress(false), selected(false), type(type), animeCount(0), rowChecked(false), colChecked(false), chaincount(0), m_mode(1), m_lockSP(0)
	{
		
	}
	bool m_locked;
	int m_mode;
	virtual bool init();
	Sprite* m_lockSP;
	const static std::string BlockTypeName[];
	void autounlock(float f);
};


#endif