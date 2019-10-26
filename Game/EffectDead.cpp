#include "EffectDead.h"



void EffectDead::Render()
{

		animations[state]->Render(-1,x, y);
}

void EffectDead::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (timeDelete == -1)
	{
		timeDelete = GetTickCount();
	}
	else
	{
		DWORD now = GetTickCount();

		if (now - timeDelete > EFFECTDEAD_TIME)
		{
			isDestroy = true;
			return;
		}
	}
	GameObject::Update(dt);
	x += dx;
	y += dy;
}

void EffectDead::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	/*l = x;
	t = y;
	if (state == 1)
	{
		r = x + 16;
		b = y + 20;
	}
	else if (state == 2)
	{
		r = x + 14;
		b = y + 28;
	}
	else
	{
		r = x + 42;
		b = y + 44;
	}*/
}

