#include "Panther.h"

Panther::Panther() : Enemy()
{
	AddAnimation("leopard_run_ani");
	AddAnimation("");
	AddAnimation("leopard_idle_ani");
	AddAnimation("leopard_jump_ani");

	isJumping = false;
	HP = 1;
	score = 200;
	attack = 2;
	respawnWaitingTime = 10000;
}

void Panther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == PANTHER_IDLE_INACTIVE)
		return;

	if (IsRespawn())
	{
		SetState(PANTHER_IDLE_INACTIVE);
		return;
	}



	vy += PANTHER_GRAVITY * dt;
	Enemy::Update(dt);


	// Check collision between zombie and ground (jumping on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	//DebugOut(L"%d ", coEvents.size());

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (state == PANTHER_ACTIVE && isJumping == false) // không va chạm với ground và chưa nhảy -> nhảy
		{
			isJumping = true;
			SetState(PANTHER_JUMP);
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		if (ny < 0)
		{
			vy = 0;

			if (state == PANTHER_JUMP)
			{				
				SetState(PANTHER_ACTIVE);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Panther::Render()
{
	if (state != PANTHER_IDLE_INACTIVE)
		animations[state]->Render(nx, x, y);
}

void Panther::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case PANTHER_ACTIVE:
		if (nx > 0)
			vx = PANTHER_RUNNING_SPEED_X;
		else 
			vx = -PANTHER_RUNNING_SPEED_X;
		vy = PANTHER_RUNNING_SPEED_Y;
		isJumping = false;
		break;
	case PANTHER_IDLE_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		isDestroy = false;
		vx = 0;
		vy = 0;
		StartRespawnTimeCounter();
		break;
	case PANTHER_IDLE:
		vx = 0;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		isDropItem = false;
		break;
	case PANTHER_JUMP:
		vy = -PANTHER_RUNNING_SPEED_Y;
	default:
		break;
	}
}

void Panther::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 27;  // (10/64)
	top = y;
	right = left + PANTHER_BBOX_WIDTH;
	bottom = top + PANTHER_BBOX_HEIGHT;
}

void Panther::GetActiveBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = entryPosition.x - PANTHER_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + PANTHER_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - PANTHER_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + PANTHER_ACTIVE_BBOX_HEIGHT;
}

void Panther::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (HP == 0)
		isDestroy = true;
}


