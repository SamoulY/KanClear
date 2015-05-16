#include "EXPBar.h"

using namespace cocos2d;


bool EXPBar::init()
{
	if (!Sprite::initWithSpriteFrameName("expbarbackground.png"))
		return false;
	addChild(m_expclip = ClippingNode::create(),2);
	m_expclip->setPosition(0, _contentSize.height);
	m_expclip->addChild(m_expstrip = Sprite::createWithSpriteFrameName("expbarstrip.png"));
	m_expstrip->setAnchorPoint(Point(0, 1));
	m_expclip->setStencil(m_expmask = Sprite::createWithSpriteFrameName("expbarmask.png"));
	m_expclip->setInverted(true);
	m_expmask->setAnchorPoint(Point(0, 1));
	addChild(m_expborder = Sprite::createWithSpriteFrameName("expbarborder.png"), 3);
	m_expborder->setAnchorPoint(Point(0, 0));
	addChild(m_explvlabel = Label::createWithTTF(TTFConfig("Resources/fonts/STZhongsong.ttf", 20), "LV"),10);
	m_explvlabel->setScaleY(0.95);
	m_explvlabel->setAnchorPoint(Point(0, 1));
	m_explvlabel->setPosition(10, _contentSize.height);
	addChild(m_explvnumber = Label::createWithTTF(TTFConfig("Resources/fonts/times.ttf", 35), "000"),10);
	m_explvnumber->setScaleY(0.95);
	m_explvnumber->setAnchorPoint(Point(1, 0.5));
	m_explvnumber->setPosition(35 + m_explvnumber->getContentSize().width / 2, 70);
	return true;
}
void EXPBar::refresh()
{
	if (m_lv >= 0)
		m_explvnumber->setString(String::createWithFormat("%d",m_lv)->_string);
	if (m_currentexp >= 0 && m_maxexp>0)
	{
		m_expmask->setRotation(-90*m_currentexp/m_maxexp);
	}
}
bool EXPBar::setexp(int currentexp, int lv, int maxexp)
{
	if (maxexp == -1)
		maxexp = m_maxexp;
	if (currentexp<0 || currentexp>maxexp)
		return false;
	m_currentexp = currentexp;
	m_maxexp = maxexp;
	if (lv != -1)
		m_lv = lv;
	refresh();
	return true;
};