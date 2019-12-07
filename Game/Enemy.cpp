#include "Enemy.h"



Enemy::Enemy()
{
	respawnTime_Start = 0;
	isRespawnWaiting = false;
	respawnWaitingTime = 0;
	isFinishRespawn = false;
	isDestroy = false;
	startTime = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	GameObject::Update(dt);
}

void Enemy::SetState(int state)
{
	GameObject::SetState(state);
}

void Enemy::StartRespawnTimeCounter()
{
	isRespawnWaiting = true;
	respawnTime_Start = GetTickCount();
}

bool Enemy::IsActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= respawnWaitingTime)
		return true;

	return false;
}

bool Enemy::IsRespawn()
{
	if (isDestroy)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > (DWORD)450)
			{
				start = false;
				startTime = 0;
				return true;
			}
		}
	}
	else
		return false;
}


void Enemy::LoseHP(int x)
{
	HP -= x;

	if (HP <= 0)
		HP = 0;
}
