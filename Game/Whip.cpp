#include "Whip.h"


Whip::Whip() : GameObject()
{
	AddAnimation("normalwhip_ani");
	AddAnimation("shortchain_ani");
	AddAnimation("longchain_ani");
	SetState(WHIP_NORMAL);
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<BreakWall*>(obj))
		{
			BreakWall * e = dynamic_cast<BreakWall*>(obj);

			float left, top, right, bottom;
			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetState(BREAK);
				e->isDestroy = true;
			}
		}

		else if (dynamic_cast<Candle *>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);
			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và nến
			{
				obj->isDestroy = true;
			}
		}
		else if (dynamic_cast<FireBall*>(obj))
		{
			FireBall * e = dynamic_cast<FireBall*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và boss
			{
				e->isDestroy= true;
			}
		}
		else if (dynamic_cast<Zombie*>(obj))
		{
			Zombie * e = dynamic_cast<Zombie*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và zombie
			{
				scoreReceived = e->GetScore();
			//	e->SetState(ZOMBIE_IDLE);
				e->LoseHP(1);
			}
		}
		else if (dynamic_cast<Panther*>(obj))
		{
			Panther * e = dynamic_cast<Panther*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và black leopard
			{
				scoreReceived = e->GetScore();
				e->LoseHP(1);		
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat * e = dynamic_cast<Bat*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và vampire bat
			{
				scoreReceived = e->GetScore();
				e->LoseHP(1);
				
			}
		}
		else if (dynamic_cast<FishMan*>(obj))
		{
			FishMan * e = dynamic_cast<FishMan*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và fishman
			{
				scoreReceived = e->GetScore();
				e->LoseHP(1);
			}
		}
		else if (dynamic_cast<Boss*>(obj))
		{
			Boss * e = dynamic_cast<Boss*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và boss
			{
				if (e->GetState() != BOSS_HURTED)
				{
					
					e->LoseHP(1);
					OutputDebugStringW(L"-1");
					e->SetState(BOSS_HURTED);
				}
				
			}
		}
	}
}

void Whip::Render()
{
	animations[state]->Render(nx,x, y);
}

void Whip::SetWhipPosition(float x, float y, bool isSit)
{

	if (isSit == true)
		y += 15.0f;

	x -= 90.0f;
	y += 1.0f;

	SetPosition(x, y);


}

void Whip::UpLevelWhip()
{
	if (state == WHIP_NORMAL) 
		state = WHIP_SHORT; 
	else if (state == WHIP_SHORT)
	{
		state = WHIP_LONG;
		isMaxLevel = true;
	}
}


bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	return AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (isMaxLevel)
	{
		if (nx < 0)
		{
			left = x + 20;
			right = left + WHIP_BBOX_WIDTH + 30;
		}
		else if (nx > 0)
		{
			left = x + 150;
			right = left + WHIP_BBOX_WIDTH + 30;
		}
	}
	else
	{
		if (nx < 0)
		{
			left = x + 50;
			right = left + WHIP_BBOX_WIDTH;
		}
		else if (nx > 0)
		{
			left = x + 150;
			right = left + WHIP_BBOX_WIDTH;
		}
	}
}

void Whip::reset()
{
	animations[state]->Reset();
}

