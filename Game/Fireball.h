#pragma once

#include "GameObject.h"

class FireBall : public GameObject
{
public:
	FireBall();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();

	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

