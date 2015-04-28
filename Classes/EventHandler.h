#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <vector>
#include <functional>  

template<class eventarg>class KEvent
{
public:
	typedef std::function<void(void*,eventarg*)> eventFuncType;
	void regist(eventFuncType);
	void unregeist(eventFuncType);
	void func(eventarg);
	KEvent(void* p) :parent(p)
	{
	}
	KEvent& operator +=(eventFuncType func)
	{
		regist(func);
		return *this;
	}
	KEvent& operator -=(eventFuncType func)
	{
		unregeist(func);
		return *this;
	}
private:
	std::vector<eventFuncType> m_funcs;
	void* parent;
};
template<class eventarg> void KEvent<eventarg>::func(eventarg args)
{
	int count = m_funcs.size();
	for (int i = 0; i < count; i++)
	{
		m_funcs.at(i)(parent,&args);
	}
}
template<class eventarg> inline void KEvent<eventarg>::regist(eventFuncType func)
{
	m_funcs.push_back(func);
}
template<class eventarg> inline void KEvent<eventarg>::unregeist(eventFuncType func)
{
	m_funcs.erase(func);
}



#endif