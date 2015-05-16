#include "CommanderInfoBar.h"
#include "EXPBar.h"
#include "Commander.h"

using namespace cocos2d;

bool CommanderInfoBar::init()
{
	if (!Sprite::init())
		return false;
	TTFConfig nameconfig = TTFConfig("Resources/fonts/STZhongsong.ttf", 28);
	TTFConfig labelconfig = TTFConfig("Resources/fonts/STZhongsong.ttf", 13);
	addChild(m_background = Sprite::createWithSpriteFrameName("portmaincommanderbar.png"));
	m_background->setAnchorPoint(Point(0, 1));
	addChild(m_expbar = EXPBar::create(),2);
	m_expbar->setAnchorPoint(Point(0, 1));
	addChild(m_expgear = Sprite::createWithSpriteFrameName("portmaingear_xl.png"), 1);
	m_expgear->setPosition(35,-33);
	m_expgear->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1), RotateBy::create(0.05, 6), NULL)));
	addChild(m_name = Label::createWithTTF(nameconfig, "Customer"));
	m_name->setAnchorPoint(Point(0, 1));
	m_name->setTextColor(Color4B(255, 255, 255, 255));
	float gearrightborder = m_expgear->getPositionX() + m_expgear->getContentSize().width / 2;
	m_name->setPosition(gearrightborder,0);
	m_name->setScaleY(0.95);

	ValueVector data = FileUtils::getInstance()->getValueVectorFromFile("Resources/strings/strings.xml");
	auto valuemap = data[0].asValueMap();
	//addChild(m_shipcountlabel = Label::createWithTTF(labelconfig, doc["shipcountlabel"].GetString()));
	addChild(m_shipcountlabel = Label::createWithTTF(labelconfig, valuemap.at("shipcountlabel").asString()));
	m_shipcountlabel->setAnchorPoint(Point(0, 1));
	m_shipcountlabel->setScaleY(0.95);
	m_shipcountlabel->setTextColor(Color4B(59, 229, 219, 255));
	m_shipcountlabel->setPosition(gearrightborder, -m_name->getContentSize().height*0.95);
	addChild(m_shipcountnumber = Label::createWithTTF(labelconfig, "0/0"));
	m_shipcountnumber->setAnchorPoint(Point(0, 1));
	m_shipcountnumber->setScaleY(0.95);
	m_shipcountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_shipcountnumber->enableOutline(m_shipcountnumber->getTextColor(), 1);
	m_shipcountnumber->setPosition(m_shipcountlabel->getContentSize().width+m_shipcountlabel->getPositionX(),m_shipcountlabel->getPositionY());
	//addChild(m_equipmentcountlabel = Label::createWithTTF(labelconfig, doc["equipmentcountlabel"].GetString()));
	addChild(m_equipmentcountlabel = Label::createWithTTF(labelconfig, valuemap.at("equipmentcountlabel").asString()));
	m_equipmentcountlabel->setAnchorPoint(Point(0, 1));
	m_equipmentcountlabel->setScaleY(0.95);
	m_equipmentcountlabel->setTextColor(Color4B(59,229,219,255));
	m_equipmentcountlabel->setPosition(gearrightborder, -m_name->getContentSize().height*0.95 - m_shipcountnumber->getContentSize().height*0.8);
	addChild(m_equipmentcountnumber = Label::createWithTTF(labelconfig, "0/0"));
	m_equipmentcountnumber->setAnchorPoint(Point(0, 1));
	m_equipmentcountnumber->setScaleY(0.95);
	m_equipmentcountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_equipmentcountnumber->enableOutline(m_equipmentcountnumber->getTextColor(),1);
	m_equipmentcountnumber->setPosition(m_equipmentcountlabel->getContentSize().width + m_equipmentcountlabel->getPositionX(), m_equipmentcountlabel->getPositionY());
	addChild(m_coincountlabel = Sprite::createWithSpriteFrameName("coin_s.png"));
	addChild(m_coincountnumber = Label::createWithTTF(labelconfig, "0"));
	m_coincountlabel->setPosition(270, -30);
	m_coincountnumber->setAnchorPoint(Point(1, 0.5));
	m_coincountnumber->setPosition(m_coincountlabel->getPosition() + Point(60, 0));
	m_coincountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_coincountnumber->enableOutline(m_coincountnumber->getTextColor(), 1);
	addChild(m_developmentcmaterialcountlabel = Sprite::createWithSpriteFrameName("development_s.png"));
	addChild(m_developmentcmaterialcountnumber = Label::createWithTTF(labelconfig, "0"));
	m_developmentcmaterialcountlabel->setPosition(m_coincountlabel->getPosition() + Point(80, 0));
	m_developmentcmaterialcountnumber->setAnchorPoint(Point(1, 0.5));
	m_developmentcmaterialcountnumber->setPosition(m_developmentcmaterialcountlabel->getPosition() + Point(60, 0));
	m_developmentcmaterialcountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_developmentcmaterialcountnumber->enableOutline(m_developmentcmaterialcountnumber->getTextColor(), 1);
	addChild(m_modifymaterialcountlabel = Sprite::createWithSpriteFrameName("modify_s.png"));
	addChild(m_modifymaterialcountnumber = Label::createWithTTF(labelconfig, "0"));
	m_modifymaterialcountlabel->setPosition(m_coincountlabel->getPosition() + Point(0, -27));
	m_modifymaterialcountnumber->setAnchorPoint(Point(1, 0.5));
	m_modifymaterialcountnumber->setPosition(m_modifymaterialcountlabel->getPosition()+Point(60,0));
	m_modifymaterialcountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_modifymaterialcountnumber->enableOutline(m_modifymaterialcountnumber->getTextColor(), 1);
	addChild(m_quickbuildcountlabel = Sprite::createWithSpriteFrameName("quickbuild_s.png"));
	addChild(m_quickbuildcountnumber = Label::createWithTTF(labelconfig, "0"));
	m_quickbuildcountlabel->setPosition(m_modifymaterialcountlabel->getPosition() + Point(80, 0));
	m_quickbuildcountnumber->setAnchorPoint(Point(1, 0.5));
	m_quickbuildcountnumber->setPosition(m_quickbuildcountlabel->getPosition() + Point(60, 0));
	m_quickbuildcountnumber->setTextColor(Color4B(255, 255, 255, 255));
	m_quickbuildcountnumber->enableOutline(m_quickbuildcountnumber->getTextColor(), 1);
	return true;
}
void CommanderInfoBar::refresh(const Commander* commander)
{
	m_expbar->setexp(commander->currentexp, commander->lv, commander->maxexp);
	m_name->setString(commander->name);
	m_shipcountnumber->setString(String::createWithFormat("%d/%d", commander->shipscount, commander->maxshipscount)->_string);
	m_equipmentcountnumber->setString(String::createWithFormat("%d/%d", commander->equipmentcount, commander->maxequipmentcount)->_string);
	m_coincountnumber->setString(String::createWithFormat("%d", commander->coincount)->_string);
	m_developmentcmaterialcountnumber->setString(String::createWithFormat("%d", commander->developmentcmaterialcount)->_string);
	m_modifymaterialcountnumber->setString(String::createWithFormat("%d", commander->modifymaterialcount)->_string);
	m_quickbuildcountnumber->setString(String::createWithFormat("%d", commander->quickbuildcount)->_string);
}
