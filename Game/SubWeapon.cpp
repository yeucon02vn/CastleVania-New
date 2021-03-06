#include "SubWeapon.h"
#include "Candle.h"
#include "Ground.h"

SubWeapon::SubWeapon()
{
	AddAnimation("");
	AddAnimation("");
	AddAnimation("");
	AddAnimation("weapon_knife_ani");
	AddAnimation("weapon_boomerang_ani");
	AddAnimation("weapon_axe_ani");
	AddAnimation("weapon_stop_watch_ani");
	AddAnimation("weapon_holywater_ani");
	AddAnimation("weapon_holywatershattered_ani");

	scoreReceived = 0;
	state = -1; // no subweapon
}


void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == WEAPON_STOP_WATCH)
		return;
	if(isColisionGround == true &&
		GetTickCount() - holyWaterShatteredCounter > HOLY_WATER_TIME_EFFECT)
	{
		isColisionGround = false;
		holyWaterShatteredCounter = 0;
		isDestroy = true;
		return;
	}

	GameObject::Update(dt);

	switch (state)
	{
	case WEAPON_AXE:
		vy += WEAPON_GRAVITY * dt;
		break;
	case WEAPON_HOLY_WATER:
		vy += WEAPON_GRAVITY * dt;
		break;
	case WEAPON_BOOMERANG:
		if (nx > 0) vx -= BOOMERANG_TURNBACK_SPEED;
		else vx += BOOMERANG_TURNBACK_SPEED;
		break;
	default:
		break;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	static int  c = 0;

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle*>(e->obj))
			{
				Candle * candle = dynamic_cast<Candle*>(e->obj);
				candle->isDestroy = true;;

			
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<FireBall*>(e->obj))
			{
				FireBall * fireball = dynamic_cast<FireBall*>(e->obj);
				fireball->isDestroy = true;

				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie * zombie = dynamic_cast<Zombie*>(e->obj);
				zombie->LoseHP(2);
				scoreReceived = zombie->GetScore();
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<Panther*>(e->obj))
			{
				Panther * panther = dynamic_cast<Panther*>(e->obj);
				panther->LoseHP(2);
				scoreReceived = panther->GetScore();
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat * bat = dynamic_cast<Bat*>(e->obj);
				bat->LoseHP(2);
				scoreReceived = bat->GetScore();
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<FishMan*>(e->obj))
			{
				FishMan * fishman = dynamic_cast<FishMan*>(e->obj);
				fishman->LoseHP(2);
				scoreReceived = fishman->GetScore();
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				};
			}
			else if (dynamic_cast<Boss*>(e->obj))
			{
				//if(boss->GetState)
				Boss * boss = dynamic_cast<Boss*>(e->obj);
				boss->LoseHP(2);
				if (state == WEAPON_KNIFE)
					isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (state == WEAPON_HOLY_WATER && e->ny == -1)
					SetState(WEAPON_HOLY_WATER_SHATTERED);

				x += dx;
				y += dy;
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				if (state == WEAPON_BOOMERANG)
					isDestroy = true;
			}
		}
	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SubWeapon::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void SubWeapon::Render()
{
	if (state == WEAPON_STOP_WATCH)
		return;
	if (this->isDestroy == false && this->state != -1)
		animations[state]->Render(nx, x, y);
}



void SubWeapon::SetState(int state)
{
	GameObject::SetState(state);

	scoreReceived = 0;

	switch (state)
	{
	case WEAPON_STOP_WATCH:
		break;
	case WEAPON_KNIFE:
		if (nx > 0) vx = KNIFE_SPEED;
		else vx = -KNIFE_SPEED;
		vy = 0;
		break;
	case WEAPON_AXE:
		if (nx > 0) vx = AXE_SPEED_X;
		else vx = -AXE_SPEED_X;
		vy = -AXE_SPEED_Y;
		break;
	case WEAPON_HOLY_WATER:
		vx = nx * HOLY_WATER_SPEED_X;
		vy = -HOLY_WATER_SPEED_Y;
		break;
	case WEAPON_HOLY_WATER_SHATTERED:
		vx = 0;
		vy = 0;
		StartHolyWaterEffect();
		break;
	case WEAPON_BOOMERANG:
		vx = nx * BOOMERANG_SPEED;
		vy = 0;
		break;
	default:
		break;
	}
}


void SubWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	switch (state)
	{

	case WEAPON_STOP_WATCH:
		right = left;  
		bottom = top;
		break;
	case WEAPON_KNIFE:
		right = left + KNIFE_BBOX_WIDTH;
		bottom = top + KNIFE_BBOX_HEIGHT;
		break;
	case WEAPON_AXE:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	case WEAPON_HOLY_WATER:
		right = left + HOLY_WATER_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	case WEAPON_BOOMERANG:
		right = left + BOOMERANG_BBOX_WIDTH;
		bottom = top + BOOMERANG_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void SubWeapon::StartHolyWaterEffect()
{
	isColisionGround = true;
	holyWaterShatteredCounter = GetTickCount();
}



