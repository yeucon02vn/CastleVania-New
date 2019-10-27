#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Candle.h"

class Whip : public GameObject
{
	bool isMaxLevel = false;
public:
	Whip();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void reset();
	void SetWhipPosition(float x, float y, bool isSit);
	void UpLevelWhip();
	bool checkLevel() { return isMaxLevel; }
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
