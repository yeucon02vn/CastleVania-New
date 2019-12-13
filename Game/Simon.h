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
#include "Debug.h"
#include <string>
class Simon : public GameObject
{
	int hp;
	int life;
	int heart;
	int score;
	D3DXVECTOR2 pos;
	int startTime;
	bool start;
	Whip *whip;

	int untouchable;
	bool isSit;
	bool MaxLevelWhip;
	DWORD untouchable_start;

	bool isFallingWater = false;
	bool isCheckColision = false;
	float autoWalkDistance = 0;		
	int stateAfterAutoWalk = -1;	
	int nxAfterAutoWalk = 0;		
public:
	bool isFinishDelayAnimation;
	bool canMoveUpStair;
	bool canMoveDownStair;
	bool isWalkThroughDoor;
	bool autoWalk;
	int changeScene;
	bool isCoGround;
	bool KillAll;
	int SubWeapon;
	bool isHitSubWeapon;
	int stairDirection = 0;  // 1: lên quay mặt bên phải, -1: lên quay mặt bên trái
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
	int GetScore() { return score; }
	int GetHP() { return hp; }
	int GetSubWeapon() { return SubWeapon; }
	int GetHeart() { return heart; }
	int GetLife() { return life; }
	void LoseLife(int life) { this->life -= life;}
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects);
	bool CheckCollisionWithItem(vector<LPITEMS> * listItems);
	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	void LoseHP(int x);
	void AutoWalk(float distance, int new_state, int new_nx);
	//void AutoWalk(float x_obj, float distance, int new_state, int new_nx);
	void DoAutoWalk();

	void DelayAnimation();
	void StandOnStair() { vx = vy = 0; }
	void ResetAniamtionHit();
};