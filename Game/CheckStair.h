#pragma once
#include "GameObject.h"
#include "Define.h"

class CheckStair : public GameObject
{

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL) {}
	virtual void Render() {}

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};


