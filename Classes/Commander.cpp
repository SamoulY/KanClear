#include "Commander.h"

Commander* Commander::m_instance = 0;

bool Commander::Load()
{
	return false;
}
bool Commander::New()
{
	m_instance = new Commander();
	return true;
}
