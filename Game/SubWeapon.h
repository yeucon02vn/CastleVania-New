#pragma once
#include "GameObject.h"

class SubWeapon : public GameObject
{

	float x_start;
public:
	SubWeapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void SetPosition(float x, float y);
	virtual void Render();
	void SetDestroy(int state) { this->isDestroy = state; }
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	

};

