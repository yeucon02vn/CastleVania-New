#include "Zombie.h"



Zombie::Zombie() : Enemy()
{
	AddAnimation("zombie_walk_ani");
	AddAnimation("");
	HP = 1;
	score = 100;
	attack = 2;
	respawnWaitingTime = 5000;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == ZOMBIE_IDLE)
		return;

	if (IsRespawn())
	{
		SetState(ZOMBIE_IDLE);
		return;
	}

	Enemy::Update(dt);

	// Check collision between zombie and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		vy += GRAVITY * dt;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}
		else if (ny == -1.0f)
		{
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Zombie::Render()
{
	if (state == ZOMBIE_ACTIVE)
		animations[state]->Render(nx, x, y);
}

void Zombie::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ZOMBIE_ACTIVE:
		if (nx > 0) vx = ZOMBIE_WALKING_SPEED;
		else vx = -ZOMBIE_WALKING_SPEED;
		vy = 0;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case ZOMBIE_IDLE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		isFinishRespawn = false;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Zombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 11;  // (10/32)
	top = y + 2; // (60,64)
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}


void Zombie::GetActiveBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = entryPosition.x - ZOMBIE_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + ZOMBIE_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - ZOMBIE_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + ZOMBIE_ACTIVE_BBOX_HEIGHT;
}

void Zombie::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (HP == 0)
		isDestroy = true;
}


