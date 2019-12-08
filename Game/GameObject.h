#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Animations.h"
#include "Define.h"
#include <algorithm>
using namespace std;

class GameObject;
typedef GameObject * LPGAMEOBJECT;


struct CollisionEvent;
typedef CollisionEvent * LPCOLLISIONEVENT;


struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};

class GameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;
	int state;

	DWORD dt;

	vector<LPANIMATION> animations;

public:
	bool isDropItem;
	bool isDestroy;
	bool isEffect;
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void getX(float &x) { x = this->x; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void setNx(int nx) { this->nx = nx; }
	int GetState() { return this->state; }

	void RenderBoundingBox();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);

	void AddAnimation(string aniId);
	bool AABB(float obj_aleft, float obj_atop, float obj_aright, float obj_abottom,
		float obj_bleft, float obj_btop, float obj_bright, float obj_bbottom);

	GameObject();


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void SetHP(int hp) {}
	~GameObject();
};
