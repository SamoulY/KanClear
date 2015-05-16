#ifndef _COINBANK_H_
#define _COINBANK_H_

#include "cocos2d.h"

class CoinBank : public cocos2d::Sprite
{
public:
	static CoinBank* create(int start = 0);
	virtual void updateCoin(int target);
	virtual void AddCoin(int target);
	virtual int getCurrentCoin()
	{
		return m_coincount;
	}
protected:
	cocos2d::Sprite* m_box;
	cocos2d::Label* m_coincountlabel;
	int m_coincount;
	virtual bool init() override;
	CoinBank(int start = 0) :m_coincount(start)
	{

	}
};

#endif