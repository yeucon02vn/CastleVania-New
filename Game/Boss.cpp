#include "Boss.h"


Boss::Boss() : Enemy()
{

	AddAnimation("boss_fly_ani");
	AddAnimation("boss_idle_ani");
	AddAnimation("boss_hurt_ani");
	std::srand(time(0));
	atSpot = true;
	aimable = false;
	waitTime = 0;
	hurtTime = 0;
	isHurt = false;
	simonPos.x = -1;

	HP = 16;
	score = 3000;
	attack = 3;
	SetState(BOSS_SLEEP);
}

void Boss::Fly(float gameTime)
{

	D3DXVECTOR2 pos;
	GetPosition(pos.x, pos.y);
	pos.x += direction.x * vx * gameTime;
	pos.y += direction.y * vy * gameTime;

	if (std::sqrt(std::pow(simonPos.x - pos.x, 2) + std::pow(simonPos.y - pos.y, 2)) >= distance)
	{
		aimable = false;
		pos.x = simonPos.x;
		pos.y = simonPos.y;
	}

	SetPosition(pos.x, pos.y);

	SetState(BOSS_ACTIVE);
}


void Boss::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 13;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y + BOSS_BBOX_HEIGHT;
}


void Boss::Render()
{
	animations[state]->Render(nx,x,y);
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (HP > 0)
	{
		if (state == BOSS_SLEEP)
			return;
		GameObject::Update(dt, coObjects);

		if (isHurt == true)
		{
			if (start == false)
			{
				start = true;
				hurtTime += GetTickCount();
			}
			else
			{
				if (GetTickCount() - hurtTime > HURT_TIME)
				{
					start = false;
					hurtTime = 0;
					isHurt = false;
					SetState(BOSS_ACTIVE);
				}
			}
		}


		else
		{
			if (aimable == true) {
				if (waitTime > 0)
					waitTime -= dt;
				else
					Fly(dt);
			}
			else {
				if (atSpot == false)
					FlyToRandomSpot(dt);
				else
					RandomSpot();
			}
		}
	}
}



void Boss::FlyToRandomSpot(float gameTime)
{

	distance = std::sqrt(
		std::pow(random.x - x, 2) +
		std::pow(random.y - y, 2));


	direction.x = (random.x - x) / distance;
	direction.y = (random.y - y) / distance;


	D3DXVECTOR2 pos;
	GetPosition(pos.x, pos.y);
	pos.x += direction.x * vx * gameTime;
	pos.y += direction.y * vy * gameTime;;


	if (std::sqrt(std::pow(random.x - pos.x, 2) + std::pow(random.y - pos.y, 2)) >= distance)
	{
		vx = 0.3f;
		vy = 0.3f;
		chaseTime = CHASE_TIME;

		aimable = true;
		atSpot = true;
		pos.x = random.x;
		pos.y = random.y;
	}

	SetPosition(pos.x, pos.y); // So we setup (valid) new position

	state = BOSS_ACTIVE; // Batman's flying so action = fly



}

void Boss::setPoisitionBoss(float x, float y)
{
	SetPosition(x, y);
	SetEntryPosition(x, y);
	preX = x - 320;
	preY = y - 10;
}
void Boss::AimSimon(LPGAMEOBJECT simon, DWORD dt)
{
	if (simonPos.x < 0) {
		simonPos = { simon->x, simon->y }; // lấy vị trí simon
	}
	else if (chaseTime >= 0) {
		chaseTime -= dt;
		simonPos = { simon->x, simon->y };
	}

	if (aimable == true)
	{
		distance = std::sqrt(
			std::pow(simonPos.x - x, 2) + // lấy khoảng cách chéo tới simon
			std::pow(simonPos.y - y, 2));

		direction.x = (simonPos.x - x) / distance; // lấy khoảng cách của tới simon theo x ,y
		direction.y = (simonPos.y - y) / distance;
	}
}

void Boss::RandomSpot()
{

	random.x = std::rand() % (600) + preX; // random trong khu vực
	random.y = std::rand() % (300) + preY;


	atSpot = false;
	waitTime = WAIT_TIME;
	vx = 0.25f;
	vy = 0.2f;
}

void Boss::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case BOSS_ACTIVE:
		break;
	case BOSS_SLEEP:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		break;
	case BOSS_HURTED:
		isHurt = true;
		break;

	}

}


void Boss::GetActiveBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = entryPosition.x + 120;
	right = left + BOSS_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y;
	bottom = entryPosition.y + BOSS_ACTIVE_BBOX_HEIGHT;
}

void Boss::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (HP == 0)
		isDestroy = true;
}

