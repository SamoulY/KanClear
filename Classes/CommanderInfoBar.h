#ifndef _COMMANDERINFOBAR_H_
#define _COMMANDERINFOBAR_H_

#include "cocos2d.h"


class Commander;
class EXPBar;


class CommanderInfoBar :public cocos2d::Sprite
{
public:
	CREATE_FUNC(CommanderInfoBar);
	void refresh(const Commander* commander);
protected:
	bool init() override;
	EXPBar* m_expbar;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_expgear;
	cocos2d::Label* m_name;
	cocos2d::Label* m_shipcountlabel;
	cocos2d::Label* m_shipcountnumber;
	cocos2d::Label* m_equipmentcountlabel;
	cocos2d::Label* m_equipmentcountnumber;
	cocos2d::Sprite* m_modifymaterialcountlabel;
	cocos2d::Label* m_modifymaterialcountnumber;
	cocos2d::Sprite* m_coincountlabel;
	cocos2d::Label* m_coincountnumber;
	cocos2d::Sprite* m_developmentcmaterialcountlabel;
	cocos2d::Label* m_developmentcmaterialcountnumber;
	cocos2d::Sprite* m_quickbuildcountlabel;
	cocos2d::Label* m_quickbuildcountnumber;

};


#endif