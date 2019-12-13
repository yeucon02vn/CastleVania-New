#pragma once
#include "Enemy.h"
#include "Textures.h"
#include "Define.h"



class Boss : public Enemy
{

	D3DXVECTOR2 direction;
	D3DXVECTOR2 random;
	D3DXVECTOR2 simonPos;

	bool start;
	bool atSpot;
	bool aimable;
	float distance;
	int hurtTime;
	bool isHurt;
	float waitTime;
	float chaseTime;
	float preX;
	float preY;
public:
	Boss();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void SetState(int state);
	void AimSimon(LPGAMEOBJECT simon, DWORD dt);
	void RandomSpot();
	void Fly(float gameTime);
	void FlyToRandomSpot(float gameTime);
	void setPoisitionBoss(float x, float y);
	void GetActiveBoundingBox(float & left, float & top, float & right, float & bottom);
	void LoseHP(int x);
};