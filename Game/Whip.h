#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Candle.h"
#include "BreakWall.h"
#include "Fireball.h"
#include "Bat.h"
#include "Zombie.h"
#include "FishMan.h"
#include "Panther.h"
#include "Boss.h"
class Whip : public GameObject
{
	int scoreReceived;;
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
