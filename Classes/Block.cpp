#include "Block.h"
using namespace cocos2d;

const std::string Block::BlockTypeName[] = {
	"Attack.png",
	"Lock.png",
	"Recover.png",
	"Mana.png",
	"Coin.png",
	"",
	"Key.png"
};
void drawLine(DrawNode* target, float x1, float y1, float x2, float y2,float stand)
{
	if (abs(x2-x1)<stand||abs(y2-y1)<stand)
	{
		target->drawLine(Point(x1, y1), Point(x2, y2), Color4F(1,1,1,0.5));
		return;
	}
	float mid_x = (x1 + x2) / 2;
	float mid_y = (y1 + y2) / 2;
	mid_x += (rand_0_1() - 0.5)*abs(x2-x1);
	mid_y += (rand_0_1() - 0.5)*abs(y2-y1);
	drawLine(target, x1, y1, mid_x, mid_y,stand);
	drawLine(target, x2, y2, mid_x, mid_y, stand);
}
void drawLighting(float dt,DrawNode* target)
{
	target->clear();
	auto size = target->getContentSize();
	int lightcount = random(2, 5);
	float p, q, rr = 0.1764*size.width*size.height, r2 = size.width *0.84;
	for (int i = 0; i < lightcount; i++)
	{
		p = (rand_0_1()-0.5)*size.width;
		q = (sqrt(rr - p*p))*(rand_0_1()>0.5 ? 1 : -1);
		drawLine(target, size.width / 2 + p, size.height / 2 + q, size.width / 2 - p, size.height / 2 - q,r2/20);
	}
}

void Splash(Sprite* target)
{
	target->runAction(RepeatForever::create(Blink::create(0.2, 1)));
}
Block* Block::create(BlockType type)
{
	Block* block = new Block(type);
	if (block&&block->init())
	{
		block->autorelease();
	}
	else
	{
		delete(block);
		block = nullptr;
	}
	return block;
}
bool Block::init()
{
	if (!Sprite::init())
		return false;
	this->initWithSpriteFrameName(BlockTypeName[type]);
	return true;
}
int Block::getMode()
{
	return m_mode;
}
void Block::setMode(int mode)
{
	if (mode < 4)
	{
		mode = 1;
	}
	if (mode == m_mode)
		return;
	m_mode = mode;
	removeAllChildrenWithCleanup(true);
	Sprite* blendSP;
	switch (m_mode)
	{
	case 5:
		blendSP = Sprite::createWithSpriteFrameName("focus.png");
		blendSP->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		blendSP->setOpacity(160);
		addChild(blendSP,5);
		blendSP->runAction(RepeatForever::create(RotateBy::create(6,360)));
	case 4:
		blendSP = Sprite::createWithSpriteFrameName("stars_1.png");
		blendSP->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		addChild(blendSP,10);
		blendSP->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create(std::bind(Splash,blendSP)), NULL));
		blendSP = Sprite::createWithSpriteFrameName("stars_2.png");
		blendSP->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		addChild(blendSP,10);
		blendSP->runAction(Sequence::create(DelayTime::create(0), CallFunc::create(std::bind(Splash, blendSP)), NULL));
		blendSP = Sprite::createWithSpriteFrameName("stars_3.png");
		blendSP->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		addChild(blendSP,10);
		blendSP->runAction(Sequence::create(DelayTime::create(0.05), CallFunc::create(std::bind(Splash, blendSP)), NULL));
		break;
	case 1:break;
	default:
		type = BlockType::Key;
		initWithSpriteFrameName(BlockTypeName[type]);
		blendSP = Sprite::createWithSpriteFrameName("Spiral.png");
		blendSP->setPosition(_contentSize.width / 2, _contentSize.height / 2);
		blendSP->setBlendFunc({ GL_ONE, GL_ONE });
		blendSP->setOpacity(200);
		addChild(blendSP, 5);
		blendSP->runAction(RepeatForever::create(RotateBy::create(6, 360)));
		break;
	}
}
void Block::lock(float time)
{
	m_locked = true;
	m_lockSP = Sprite::createWithSpriteFrameName("Locked.png");
	m_lockSP->setPosition(_contentSize.width/2, _contentSize.height/2);
	auto action = RepeatForever::create(Sequence::create(ScaleBy::create(0.4, 1.25), ScaleBy::create(0.4, 0.8), NULL));
	action->setTag(99);
	m_lockSP->runAction(action);
	setColor(Color3B(150,150,150));
	int count = _children.size();
	for (int i = 0; i < count; i++)
	{
		_children.at(i)->setColor(Color3B(150, 150, 150));
		_children.at(i)->pauseSchedulerAndActions();
	}
	addChild(m_lockSP, 15);
	m_lockSP->setPosition(_contentSize.width * 4 / 5, _contentSize.height * 1 / 4);
	scheduleOnce(schedule_selector(Block::autounlock), time);
}
void Block::unlock()
{
	m_lockSP->runAction(Sequence::create(ScaleTo::create(0.3, 1.5), CallFunc::create([this]
	{
		m_locked = false;
		m_lockSP->removeFromParent();
		setColor(Color3B(255, 255, 255));
		int count = _children.size();
		for (int i = 0; i < count; i++)
		{
			_children.at(i)->setColor(Color3B(255, 255, 255));
			_children.at(i)->resumeSchedulerAndActions();
		}
	}),NULL));
}
void Block::autounlock(float f)
{
	if (m_locked)
		unlock();
}
