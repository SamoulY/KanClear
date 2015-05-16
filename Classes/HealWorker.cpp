#include "HealWorker.h"
using namespace cocos2d;

bool HealWorker::init()
{
	if (!Sprite::initWithSpriteFrameName("RecoverWorker_0.png"))
		return false;
	setVisible(false);
	m_originposition =Point(0,0);
	return true;
}
void HealWorker::work(Point targetP, float gtime, float animetime, float btime)
{
	if (gtime == -1)
		gtime = 0.1;
	if (btime == -1)
		btime = abs(targetP.x - m_originposition.x)*0.003;
	if (animetime == -1)
		animetime = 0.8;
	stopAllActions();
	setVisible(true);
	setSpriteFrame("RecoverWorker_0.png");
	runAction(Sequence::create(MoveTo::create(gtime, targetP), CallFunc::create([this]{setSpriteFrame("RecoverWorker_1.png"); }), DelayTime::create(animetime / 4),
		CallFunc::create([this]{setSpriteFrame("RecoverWorker_2.png"); }), DelayTime::create(animetime / 4),
		CallFunc::create([this]{setSpriteFrame("RecoverWorker_1.png"); }), DelayTime::create(animetime / 4),
		CallFunc::create([this]{setSpriteFrame("RecoverWorker_2.png"); }), DelayTime::create(animetime / 4),
		CallFunc::create([this]{setSpriteFrame("RecoverWorker_3.png"); }), MoveTo::create(btime, m_originposition), Hide::create(), NULL));
}