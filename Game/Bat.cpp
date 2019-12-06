#include "Bat.h"



Bat::Bat() : Enemy()
{
	AddAnimation("vampirebat_fly_ani");

	velocityVariation = 0.004f;

	HP = 1;
	score = 200;
	attack = 2;
	respawnWaitingTime = 5000;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == BAT_IDLE)
		return;

	if (IsRespawn())
	{
		SetState(BAT_IDLE);
		return;
	}


	Enemy::Update(dt);

	vy += velocityVariation;

	if (vy >= BAT_FLYING_SPEED_Y || vy <= -BAT_FLYING_SPEED_Y)
		velocityVariation *= -1;

	x += dx;
	y += dy;
}

void Bat::Render()
{
	if (state != BAT_IDLE)
		animations[state]->Render(1, nx, x, y);
}

void Bat::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case BAT_ACTIVE:
		if (nx > 0) vx = BAT_FLYING_SPEED_X;
		else vx = -BAT_FLYING_SPEED_X;
		vy = 0;
		isDropItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case BAT_IDLE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		isFinishRespawn = false;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Bat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 11;
	top = y + 11;
	right = left + BAT_BBOX_WIDTH;
	bottom = top + BAT_BBOX_HEIGHT;
}

void Bat::GetActiveBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = entryPosition.x - BAT_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - BAT_ACTIVE_BBOX_HEIGHT;
	right = entryPosition.x + BAT_ACTIVE_BBOX_WIDTH;
	bottom = entryPosition.y + BAT_ACTIVE_BBOX_HEIGHT;
}

void Bat::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (HP == 0)
		isDestroy = true;
}
