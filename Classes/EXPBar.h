#ifndef _EXPBAR_H_
#define _EXPBAR_H_

#include "cocos2d.h"

class EXPBar : public cocos2d::Sprite
{
public:
	CREATE_FUNC(EXPBar);
	int getcurrentexp()
	{
		return m_currentexp;
	}
	int getmaxexp()
	{
		return m_maxexp;
	}
	bool setexp(int currentexp, int lv = -1, int maxexp = -1);
protected:
	EXPBar():m_maxexp(-1),m_currentexp(-1),m_lv(0){};
	bool init() override;
	int m_maxexp;
	int m_currentexp;
	int m_lv;
	cocos2d::Label* m_explvnumber;
	cocos2d::Label* m_explvlabel;
	cocos2d::Sprite* m_expborder;
	cocos2d::ClippingNode* m_expclip;
	cocos2d::Sprite* m_expstrip;
	cocos2d::Sprite* m_expmask;
	void refresh();
};


#endif