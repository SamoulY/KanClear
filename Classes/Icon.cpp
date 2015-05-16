#include "Icon.h"

Icon* Icon::create(std::string charactername,IconType type)
{
	Icon* gl = new Icon(charactername,type);
	if (gl!=NULL&&gl->init())
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
bool Icon::init()
{
	if (!Sprite::initWithSpriteFrameName("characterflame.png"))
		return false;		
	std::string name;
	switch (m_type)
	{
	case IconType::Sinked:
		name = m_charactername + "BattleIconS.png";
		break;
	default:
		name = m_charactername + "BattleIcon.png";
		break;
	}
	if (m_graph != NULL)
		removeChild(m_graph, true);
	addChild(m_graph = Sprite::createWithSpriteFrameName(name));
	m_graph->setPosition(_contentSize / 2);
	return true;
}
void Icon::setIconType(IconType type)
{
	if (m_type != type)
	{
		m_type = type;
		std::string name;
		switch (type)
		{
		case IconType::Sinked:
			name = m_charactername + "BattleIconS.png";
			break;
		default:
			name = m_charactername + "BattleIcon.png";
			break;
		}
		if (m_graph != NULL)
			removeChild(m_graph, true);
		addChild(m_graph = Sprite::createWithSpriteFrameName(name));
		m_graph->setPosition(_contentSize / 2);
	}
}