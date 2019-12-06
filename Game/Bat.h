#pragma once

#include "Enemy.h"

class Bat : public Enemy
{
	float velocityVariation;

public:
	Bat();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void GetActiveBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void LoseHP(int x);
};

