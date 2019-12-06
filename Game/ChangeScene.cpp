#include "ChangeScene.h"

void ChangeScene::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + CHANGSCENE_BBOX;
	b = t + CHANGSCENE_BBOX;
}

