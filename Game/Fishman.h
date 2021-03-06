#pragma once

#include "Grid.h"
#include "Enemy.h"

class FishMan : public Enemy
{
	DWORD lastTimeShoot; // thời gian kể từ lúc vừa bắn xong đến lần bắn tiếp theo
	DWORD deltaTimeToShoot; // khoảng thời gian kể từ lúc fishman xuất hiện đến lúc bắn 
	int nxAfterShoot; // Hướng quay mặt sau khi bắn (để luôn quay mặt về phía Simon)
	bool fStart;
	int fStartTime;

public:
	FishMan();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void GetActiveBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void LoseHP(int x);

	bool CanHit();
	void Hit(Grid * grid, int new_nx);
};

