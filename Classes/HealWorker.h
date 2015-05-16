#ifndef _HEALWORKER_H_
#define _HEALWORKER_H_

#include "cocos2d.h"

class HealWorker :public cocos2d::Sprite
{
public:
	void setOriginPosion(cocos2d::Point targetP,bool alsoposition)
	{
		m_originposition = targetP;
		if (alsoposition)
			setPosition(targetP);
	}	
	void setOriginPosion(float x,float y, bool alsoposition)
	{
		m_originposition = cocos2d::Point(x,y);
		if (alsoposition)
			setPosition(x,y);
	}
	cocos2d::Point getOriginPosion()
	{
		return m_originposition;
	}
	void work(cocos2d::Point targetP, float gtime = -1, float animetime = -1, float btime = -1);
	CREATE_FUNC(HealWorker);
protected:
	bool init() override;
	cocos2d::Point m_originposition;
};


#endif