#include "SubWeapon.h"
#include "Candle.h"
#include "Ground.h"

SubWeapon::SubWeapon()
{
	
	AddAnimation("weapon_knife_ani");

	state = -1; // no subweapon
}


void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	if (abs(x - x_start) > 400)
	{
		isDestroy = true;
	}
	GameObject::Update(dt);


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
				candle->isDestroy = true;


				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<BreakWall*>(e->obj))
			{
				BreakWall * breakwall = dynamic_cast<BreakWall*>(e->obj);
				breakwall->SetState(BREAK);
				breakwall->isDestroy = true;

				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie * zombie = dynamic_cast<Zombie*>(e->obj);
				zombie->LoseHP(2);;


				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
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


				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
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


				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
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


				if (state == WEAPON_KNIFE)
					this->isDestroy = true;
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
					this->isDestroy = true;
				else
				{
					x += dx;
					y += dy;
				}
			}


		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SubWeapon::SetPosition(float x, float y)
{
	x_start = x;
	this->x = x;
	this->y = y;
}

void SubWeapon::Render()
{
	if (this->isDestroy == false && this->state != -1)
		animations[0]->Render(nx, x, y);
}



void SubWeapon::SetState(int state)
{
	GameObject::SetState(state);


	switch (state)
	{

	case WEAPON_KNIFE:
		if (nx > 0) vx = WEAPON_KNIFE_SPEED;
		else vx = -WEAPON_KNIFE_SPEED;
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

	case WEAPON_KNIFE:
		right = left + WEAPON_KNIFE_BBOX_WIDTH;
		bottom = top + WEAPON_KNIFE_BBOX_HEIGHT;
		break;
	
	default:
		right = left;
		bottom = top;
		break;
	}
}



