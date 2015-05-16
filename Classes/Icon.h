#ifndef _ICON_H_
#define _ICON_H_


#include "cocos2d.h"

enum IconType
{
	Normal,
	Sinked
};
class Icon :public cocos2d::Sprite
{
public:
	static Icon* create(std::string charactername,IconType type = IconType::Normal);
	void setIconType(IconType type);
	IconType getCurrentIconType()
	{
		return m_type;
	}
protected:
	IconType m_type;
	std::string m_charactername;
	cocos2d::Sprite* m_graph;
	virtual bool init() override;
	Icon(std::string charactername, IconType type) :m_charactername(charactername), m_type(type), m_graph(0)
	{

	}
};



#endif