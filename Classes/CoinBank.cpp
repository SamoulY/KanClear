#include "CoinBank.h"

using namespace cocos2d;

CoinBank* CoinBank::create(int start)
{
	CoinBank* gl = new CoinBank(start);
	if (gl&&gl->init())
	{
		gl->autorelease();
	}
	else
	{
		delete gl;
		gl = NULL;
	}
	return gl;
}

bool CoinBank::init()
{
	if (!Sprite::initWithSpriteFrameName("bankbuttom.png"))
		return false;
	addChild(m_coincountlabel = Label::createWithTTF(TTFConfig("Resources/fonts/STZhongsong.ttf", 20), CCString::createWithFormat("%d",m_coincount)->_string));
	m_coincountlabel->setAnchorPoint(Point(1, 0));
	m_coincountlabel->setPosition(_contentSize.width - 4, 3);
	m_coincountlabel->setTextColor(Color4B(125,125,125,255));
	addChild(m_box = Sprite::createWithSpriteFrameName("bank.png"));
	m_box->setAnchorPoint(Point(0.5, 0));
	m_box->setPosition(_contentSize.width / 2, _contentSize.height - m_box->getContentSize().height);
	return true;
}
void CoinBank::updateCoin(int target)
{
	FiniteTimeAction* temp;
	m_box->runAction(Sequence::create(temp = ScaleBy::create(0.2,1.2,0.7),temp->reverse(),NULL));
	m_coincount = target;
	m_coincountlabel->setString(CCString::createWithFormat("%d", m_coincount)->_string);
}
void CoinBank::AddCoin(int target)
{
	updateCoin(m_coincount + target);
}