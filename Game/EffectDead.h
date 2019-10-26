#pragma once
#include "GameObject.h"
#include "Textures.h"


class EffectDead : public GameObject
{
	DWORD timeDelete;
public:
	//bool isSplashing;
	EffectDead() { timeDelete = -1; AddAnimation("effect_fire_ani"); AddAnimation("effect_star_ani"); }
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};