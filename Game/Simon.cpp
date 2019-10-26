#include <algorithm>
#include "debug.h"
#include "Simon.h"
#include "Game.h"

Simon::Simon() : GameObject()
{

	AddAnimation("simon_stand_idle_ani");
	AddAnimation("simon_walk_ani");
	AddAnimation("simon_sit_ani");
	AddAnimation("simon_jump_ani");
	AddAnimation("simon_hitsit_ani");
	AddAnimation("simon_hitstand_ani");
	AddAnimation("simon_effect_ani");

	SetState(SIMON_STAND_IDLE);
	whip = new Whip();
	hp = 16;
	life = 3;
	start = false;
	isCoGround = false;
	MaxLevelWhip = false;
	SubWeapon = -1;
}



void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 


	vy += GRAVITY * dt;
	GameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();





	if (state == SIMON_EFFECT)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > SIMON_EFFECT_TIME)
			{
				start = false;
				remain = 0;
				startTime = 0;
				SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

	if (isAttack())
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > SIMON_ATTACK_TIME)
			{
				start = false;
				remain = 0;
				startTime = 0;
				reset();
				isHitSubWeapon = false;
				if (isSit)
					this->SetState(SIMON_SIT);
				else
					this->SetState(SIMON_STAND_IDLE);
				whip->reset();
			}
		}
	}

	if (isAttack())
	{
		whip->setNx(nx);
		whip->SetWhipPosition(x, y, isSit);
		if (animations[state]->getCurrentFrame() == animations[state]->getLastFrame()) // chỉ xét va chạm khi render tới sprite cuối cùng của roi
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT e = coObjects->at(i);

				if (dynamic_cast<Candle *>(e))
				{
					float left, top, right, bottom;
					e->GetBoundingBox(left, top, right, bottom);
					if (whip->CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và nến
					{
						e->isDestroy = true;
					}
				}	
			}
		}
	}
	// turn off collision when die 
	if (state != SIMON_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 


		//x += min_tx * dx + nx * 0.1f;
		//y += min_ty * dy + ny * 0.1f;


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// collision of Simon and Candle -> do nothing -> update x;
			if (dynamic_cast<Ground *>(e->obj))
			{
				if (e->ny != 0)
				{					
					if (e->ny == -1.0f)
					{
						vy = 0;
						isCoGround = true;
					}
					else
						y += dy;
				}

				x += dx;


			}
			else if (dynamic_cast<Candle *>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
				{
					if (e->ny == -1.0f)
						y += dy;
					else
						vy = 0;
				}

			}
			else if (dynamic_cast<Items *>(e->obj))
			{


				if (e->obj->GetState() == ITEM_WHIP)
				{
					SetState(SIMON_EFFECT);
					if (whip->GetState() == WHIP_NORMAL || whip->GetState() == WHIP_SHORT) whip->UpLevelWhip();
					if (whip->checkLevel())
						MaxLevelWhip = true;
				}
				else if (e->obj->GetState() == ITEM_KNIFE)
				{
					SubWeapon = WEAPON_KNIFE;
				}
				else if (e->obj->GetState() == ITEM_BIG_HEART)
				{
					heart += 5;
				}
				else if (e->obj->GetState() == ITEM_SMALL_HEART)
				{
					heart += 1;
				}

				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
				{
					if (e->ny == -1.0f)
						y += dy;
					else
						vy = 0;
				}
				e->obj->isDestroy = true;

			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	if (isAttack() && !isHitSubWeapon)
	{
		whip->Render();
	}
	

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[state]->Render(nx, x, y, alpha);
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case SIMON_STAND_IDLE:
		isCoGround = true;
		vx = 0;
		isSit = false;
		break;
	case SIMON_WALK:
		isCoGround = true;
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;
		break;
	case SIMON_JUMP:
		isCoGround = false;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_SIT:
		isSit = true;
		vx = 0;
		vy = 0;
		break;
	case SIMON_EFFECT:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}


}


void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 15;
	top = y;
	right = x + SIMON_BBOX_WIDTH + 2;
	bottom = y + SIMON_BBOX_HEIGHT;

}




void Simon::reset()
{
	animations[state]->Reset();
}
