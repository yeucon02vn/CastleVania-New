#pragma once
#include "GameObject.h"
#include "Zombie.h"
#include "Panther.h"
#include "Fishman.h"
#include "Fireball.h"
#include "Bat.h"
#include "BreakWall.h"
#include "Boss.h"
class SubWeapon : public GameObject
{

	float x_start;
public:
	SubWeapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void SetPosition(float x, float y);
	virtual void Render();
	void SetDestroy(int state) { this->isDestroy = state; }
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	

};

