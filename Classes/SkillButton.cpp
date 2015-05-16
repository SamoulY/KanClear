#include "SkillButton.h"
#include "Skills.h"

using namespace cocos2d;

static cocos2d::Color3B skillbuttoncolors[] = {Color3B(50,129,196),Color3B(255,129,196),Color3B(255,210,0),Color3B(255,79,211)};


SkillButton::SkillButton(const std::vector<const ActiveSkill*>& skills) :m_mana(0), m_manamax(m_skillcount * 30), m_skillcount(skills.size()), m_currentskill(0), m_buttonr2(0), SkillExcuteEvent(this), m_currentskillindex(-1), m_touched(false), m_currentskillrotater(0)
{
	for (int i = 0; i < m_skillcount; i++)
		m_skillsnames.push_back(skills[i]->getName());
}
SkillButton* SkillButton::create(const std::vector<const ActiveSkill*>& skills)
{
	SkillButton* gl = new SkillButton(skills);
	if (gl != NULL&&gl->init())
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
void roate(Sprite* target)
{
	target->runAction(Sequence::create(DelayTime::create(1.4), RotateBy::create(0.1, 6), CallFunc::create(std::bind(roate, target)), NULL));
}
bool SkillButton::init()
{
	if (!Sprite::initWithSpriteFrameName("skillbuttonbackground.png"))
		return false;
	Sprite* temp;
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttondecoration.png"));
	temp->setAnchorPoint(Point(0, 0));
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttonborder.png"),15);
	temp->setAnchorPoint(Point(0, 0));
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttontop2.png"),10);
	temp->setAnchorPoint(Point(0, 0));
	addChild(m_skillgear = Sprite::createWithSpriteFrameName("skillbuttongear.png"), 5);
	Point gearP;
	m_skillgear->setPosition(gearP = Point(_contentSize.width - m_skillgear->getContentSize().width / 2, _contentSize.height / 2));
	addChild(m_skilliconclip = ClippingNode::create(), 3);
	m_skilliconclip->setPosition(gearP);
	m_skilliconclip->setStencil(m_skilliconmask = createWithSpriteFrameName("skillicontemplate.png"));
	m_skilliconclip->setAlphaThreshold(0.2);

	temp = 0;
	m_buttonr2 = pow(m_buttonr = m_skilliconmask->getContentSize().width / 2, 2);
	int i;

	addChild(m_skillclip = ClippingNode::create(), 5);
	m_skillclip->addChild(m_skillstrip = Sprite::createWithSpriteFrameName("skillbuttonstrip.png"));
	m_skillclip->setPosition(Point(10 + m_skillstrip->getContentSize().width/2, (_contentSize.height - m_skillstrip->getContentSize().height) / 2));
	m_skillclip->setStencil(m_skillmask = Sprite::createWithSpriteFrameName("skillbuttonstrip.png"));
	m_skillmask->setAnchorPoint(Point(0.5, 0));
	m_skillclip->setInverted(true);
	m_skillstrip->setAnchorPoint(Point(0.5, 0));
	for (i = 0; i < m_skillcount - 1; i++)
	{
		temp = Sprite::createWithSpriteFrameName("skillbuttonedge.png");
		addChild(temp, 1);
		temp->setAnchorPoint(Point(0, 0.5));
		temp->setPosition(0, m_skillstrip->getContentSize().height*(i + 1) / m_skillcount+m_skillstrip->getPositionY());
	}

	addChild(m_skillcircle1 = Sprite::createWithSpriteFrameName("skillbuttoncircle.png"),20);
	m_skillcircle1->setPosition(gearP);
	m_skillcircle1->setScale(0);
	addChild(m_skillcircle2 = Sprite::createWithSpriteFrameName("skillbuttoncircle.png"),20);
	m_skillcircle2->setPosition(gearP);
	m_skillcircle2->setScale(0);
	float repeattime = 2;
	m_skillcircle1->runAction(RepeatForever::create(Sequence::create(Spawn::create(ScaleTo::create(repeattime, 1.5), EaseSineIn::create(FadeOut::create(repeattime)), NULL), Spawn::create(ScaleTo::create(0, 0),FadeIn::create(0),NULL), NULL)));
	temp = m_skillcircle2;
	m_skillcircle2->runAction(Sequence::create(DelayTime::create(repeattime / 2), CallFunc::create([temp, repeattime]{
		temp->runAction(RepeatForever::create(Sequence::create(Spawn::create(ScaleTo::create(repeattime, 1.5), EaseSineIn::create(FadeOut::create(repeattime)), NULL), Spawn::create(ScaleTo::create(0, 0), FadeIn::create(0), NULL), NULL)));
	}), NULL));
	m_skillcircle1->setVisible(false);
	m_skillcircle2->setVisible(false);
	roate(m_skillgear);
	scheduleUpdate();
	return true;
}
bool SkillButton::increaseMana(int target)
{
	if (target != 0)
	{
		int last = m_mana;
		m_mana += target;
		if (m_mana > m_manamax)
			m_mana = m_manamax;
		else if (m_mana < 0)
			m_mana = 0;
		if (m_mana != last)
		{
			float offset = m_skillmask->getContentSize().height*(m_mana-last)/m_manamax;
			m_skillmask->runAction(MoveBy::create(0.5, Point(0, offset)));
			check();
			return true;
		}
	}
	return false;
}
bool SkillButton::decreaseMana(int target)
{
	if (target != 0)
	{
		int last = m_mana;
		m_mana -= target;
		if (m_mana > m_manamax)
			m_mana = m_manamax;
		else if (m_mana < 0)
			m_mana = 0;
		if (m_mana != last)
		{
			float offset = m_skillmask->getContentSize().height*(m_mana - last) / m_manamax;
			m_skillmask->runAction(MoveBy::create(0.5, Point(0, offset)));
			check();
			return true;
		}
	}
	return false;
}
void SkillButton::update(float dt)
{
	int ch = m_skillmask->getPositionY() / m_skillmask->getContentSize().height*m_skillcount;
	m_skillstrip->setColor(skillbuttoncolors[ch]);
}
void SkillButton::onTouchBegan(Point location)
{
	auto p = location - m_skillgear->getPosition();
	if (p.x*p.x + p.y*p.y <= m_buttonr2&&!m_touched&&m_currentskill!=NULL)
	{
		m_touched = true;
		m_currentskill->setScale(0.95);
		m_currentskill->setColor(Color3B(200, 200, 200));
	}
}
void SkillButton::onTouchEnded(cocos2d::Point location)
{
	auto p = location - m_skillgear->getPosition();
	if (p.x*p.x + p.y*p.y <= m_buttonr2&&m_touched&&m_currentskill != NULL)
	{
		m_touched = false;
		m_currentskill->setScale(1);
		m_currentskill->setColor(Color3B(255, 255, 255));
		excuteSkill();
	}
}
void SkillButton::onTouchCancelled(cocos2d::Point location)
{
	if (m_currentskill != NULL&&m_touched)
	{
		m_currentskill->setScale(1);
		m_currentskill->setColor(Color3B(255, 255, 255));
		m_touched = false;
	}
}
void SkillButton::onTouchMoved(cocos2d::Point location)
{
	auto p = location - m_skillgear->getPosition();
	if (m_touched&&p.x*p.x + p.y*p.y > m_buttonr2)
	{
		m_touched = false;
		if (m_currentskill != NULL)
		{
			m_currentskill->setScale(1);
			m_currentskill->setColor(Color3B(255, 255, 255));
		}
	}
}
void SkillButton::excuteSkill()
{
	int index = getCurrentSkillIndex();
	if (index >= 0)
	{
		SkillExcuteEventArg arg(index);
		SkillExcuteEvent.func(arg);
		decreaseMana((index + 1) * 30);
	}
}
void SkillButton::check()
{
	int index = getCurrentSkillIndex();
	Sprite* temp;
	if (index < 0)
	{
		if (m_currentskill != NULL)
		{
			temp = m_currentskillrotater;
			m_currentskillrotater->runAction(Sequence::create(EaseSineOut::create(RotateBy::create(0.5, 60)), CallFunc::create([temp]{temp->removeFromParent(); }), NULL));
			m_currentskill = NULL;
			m_currentskillrotater = NULL;
			m_currentskillindex = index;
			m_skillcircle1->setVisible(false);
			m_skillcircle2->setVisible(false);
			m_touched = false;
		}
	}
	else if (index != m_currentskillindex)
	{
		if (m_currentskill != NULL)
		{
			temp = m_currentskillrotater;
			m_currentskillrotater->runAction(Sequence::create(EaseSineOut::create(RotateBy::create(0.5, 60)), CallFunc::create([temp]{temp->removeFromParent(); }), NULL));
		}
		m_skilliconclip->addChild(m_currentskillrotater = Sprite::create(), 20);
		m_currentskillrotater->addChild(m_currentskill = Sprite::createWithSpriteFrameName(String::createWithFormat("skill_%s.png", m_skillsnames[index].c_str())->_string));
		m_currentskillrotater->setPosition(0, -m_buttonr * 2);
		m_currentskill->setPosition(0, m_buttonr * 2);
		m_currentskillrotater->setRotation(-60);
		m_skillcircle1->setVisible(true);
		m_skillcircle1->setColor(skillbuttoncolors[index]);
		m_skillcircle2->setVisible(true);
		m_skillcircle2->setColor(skillbuttoncolors[index]);
		m_currentskillrotater->runAction(EaseSineOut::create(RotateBy::create(0.5, 60)));
		m_currentskillindex = index;
		m_touched = false;
	}
}
SkillButtonSimple* SkillButtonSimple::create(const std::vector<const ActiveSkill*>& skills)
{
	SkillButtonSimple* gl = new SkillButtonSimple(skills);
	if (gl != NULL&&gl->init())
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
bool SkillButtonSimple::init()
{
	if (!Sprite::initWithSpriteFrameName("skillbuttonSbackground.png"))
		return false;
	Sprite* temp;
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttonSdecoration.png"));
	temp->setAnchorPoint(Point(0, 0));
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttonSborder.png"), 15);
	temp->setAnchorPoint(Point(0, 0));
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttonSgear.png"), 5);
	temp->setPosition(temp->getContentSize().width / 2, _contentSize.height - temp->getContentSize().height / 2);
	temp->runAction(RepeatForever::create(RotateBy::create(10, 360)));
	temp->setScale(0.8);
	addChild(temp = Sprite::createWithSpriteFrameName("skillbuttonSgear.png"), 5);
	temp->setPosition(temp->getContentSize().width / 2, temp->getContentSize().height / 2);
	temp->runAction(RepeatForever::create(RotateBy::create(10, 360)));
	temp->setScale(0.8);
	

	int i;
	addChild(m_skillclip = ClippingNode::create(), 5);
	m_skillclip->addChild(m_skillstrip = Sprite::createWithSpriteFrameName("skillbuttonSstrip.png"));
	m_skillclip->setPosition(Point(12 + m_skillstrip->getContentSize().width/2, (_contentSize.height - m_skillstrip->getContentSize().height) / 2));
	m_skillclip->setStencil(m_skillmask = Sprite::createWithSpriteFrameName("skillbuttonSstrip.png"));
	m_skillmask->setAnchorPoint(Point(0.5, 0));
	m_skillclip->setInverted(true);
	m_skillstrip->setAnchorPoint(Point(0.5, 0));
	for (i = 0; i < m_skillcount - 1; i++)
	{
		temp = Sprite::createWithSpriteFrameName("skillbuttonSedge.png");
		addChild(temp, 1);
		temp->setAnchorPoint(Point(0, 0.5));
		temp->setPosition(0, m_skillstrip->getContentSize().height*(i + 1) / m_skillcount + m_skillstrip->getPositionY());
	}
	scheduleUpdate();
	return true;
}