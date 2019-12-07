#include "CheckStair.h"

void CheckStair::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + CHECKSTAIR_BBOX;
	b = t + CHECKSTAIR_BBOX;
}

void CheckStair::SetState(int state)
{
	GameObject::SetState(state);
}


