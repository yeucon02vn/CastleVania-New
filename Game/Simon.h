#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Ground.h"
#include "Candle.h"
#include "Items.h"
#include "Door.h"
#include "ChangeScene.h"
#include "Enemy.h"
#include "Bat.h"
#include "Zombie.h"
#include "Panther.h"
#include "Fishman.h"
#include "Fireball.h"
#include "Water.h"
class Simon : public GameObject
{
	int hp;
	int life;
	int heart;
	D3DXVECTOR2 pos;
	int startTime;
	bool start;
	Whip *whip;

	int untouchable;
	int action;
	bool isSit;
	bool MaxLevelWhip;
	DWORD untouchable_start;


	float autoWalkDistance = 0;		
	int stateAfterAutoWalk = -1;	
	int nxAfterAutoWalk = 0;		

public:
	bool canMoveUpStair;
	bool canMoveDownStair;
	bool isWalkThroughDoor;
	bool autoWalk;
	int changeScene;
	bool isCoGround;
	bool KillAll;
	int SubWeapon;
	bool isHitSubWeapon;
	int stairDirection = 0;
	bool isStandOnStair = false;
	D3DXVECTOR2 goToStair;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void reset();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	bool isAttack();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isMaxLevelWhip() { return MaxLevelWhip; }
	Simon();
	int GetHP() { return hp; }
	int GetSubWeapon() { return SubWeapon; }
	int GetHeart() { return heart; }
	int GetLife() { return life; }
	void DoAutoWalk();
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects);
	bool CheckCollisionWithItem(vector<LPITEMS> * listItems);
	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	void LoseHP(int x);
	void AutoWalk(float distance, int new_state, int new_nx);
	
	
	void StandOnStair() { vx = vy = 0; }
};