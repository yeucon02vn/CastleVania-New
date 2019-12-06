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
	heart = 15;
	start = false;
	isCoGround = false;
	MaxLevelWhip = false;
	SubWeapon = -1;
	autoWalk = false;
	startTime = 0;
}



void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 


	vy += GRAVITY * dt;
	GameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (autoWalk)
		DoAutoWalk();



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

			if (remain > (DWORD)SIMON_ATTACK_TIME)
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
			if (dynamic_cast<Items *>(e->obj))
			{

				OutputDebugString(L"abc");
				
			}
			else if (dynamic_cast<Ground *>(e->obj))
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
					if (e->nx != 0)
					{
						x += min_tx * dx + nx * 0.1f;
						vx = 0;
					}
					else {
						x += dx;
					}
				}

				
					


			}
			else if (dynamic_cast<Candle *>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
				{
					if (e->ny == 1.0f)
					{
						y += dy;
						x += dx;
					}
					else
						vy = 0;
				}

			}
			else if (dynamic_cast<Door *>(e->obj))
			{
				auto door = dynamic_cast<Door*>(e->obj);

				if (door->GetState() == DOOR2_IDLE)
				{
					vx = 0;

					if (e->nx > 0)
						x += 1.0f;
					else
					{
						door->SetState(DOOR2_OPEN);
						isWalkThroughDoor = true;
					}
				}
				else if (door->GetState() == DOOR1)	// đi qua cửa của scene 1
				{
					autoWalk = true;
				}
			}
			else if (dynamic_cast<LPCHANGESCENE>(e->obj))
			{
				x += dx;
				y += dy;

				ChangeScene * obj = dynamic_cast<LPCHANGESCENE>(e->obj);
				changeScene = obj->GetIdScene();
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	whip->setNx(nx);
	whip->SetWhipPosition(x, y, isSit);
	if (isAttack())
	{
		if (animations[state]->getCurrentFrame() == animations[state]->getLastFrame()) // chỉ xét va chạm khi render tới sprite cuối cùng của roi
		{
			whip->Update(dt, coObjects);
		}
	}
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




bool Simon::CheckCollisionWithItem(vector<LPITEMS> * listItems)
{
	float simon_l, simon_t, simon_r, simon_b;
	float item_l, item_t, item_r, item_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listItems->size(); i++)
	{
		if (listItems->at(i)->isDestroy == true)
			continue;

		listItems->at(i)->GetBoundingBox(item_l, item_t, item_r, item_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, item_l, item_t, item_r, item_b) == true)
		{
			listItems->at(i)->isDestroy = true;

			int idItem = listItems->at(i)->GetState();

			switch (idItem)
			{		
			case ITEM_KNIFE:
			/*case AXE:
			case STOP_WATCH:
			case HOLY_WATER:
			case BOOMERANG:*/
				SubWeapon = idItem;
				break;
			case ITEM_SMALL_HEART:
				heart += 1;
				break;
			case ITEM_BIG_HEART:
				heart += 5;
				break;
			/*case CROSS:
				isGotCrossItem = true;
				break;
			case INVISIBILITY_POTION:
				invisibilityTimer->Start();
				break;*/
			case ITEM_WHIP:
				SetState(SIMON_EFFECT);
				vx = 0;
				whip->UpLevelWhip();
				if (whip->checkLevel())
					MaxLevelWhip = true;
				break;
			/*case MONEY_BAG_RED:
				score += 100;
				break;
			case MONEY_BAG_BLUE:
				score += 400;
				break;
			case MONEY_BAG_WHITE:
				score += 700;
				break;
			case MONEY_BAG_FLASHING:
				score += 1000;
				break;
			case DOUBLE_SHOT:
				item = idItem;
				isGotDoubleShotItem = true;
				break;
			case TRIPLE_SHOT:
				item = idItem;
				isGotTripleShotItem = true;
				break;
			case PORK_CHOP:
				HP += 2;
				if (HP > SIMON_HP)
					HP = SIMON_HP;
				break;
			case MAGIC_CRYSTAL:
				isGotMagicCrystalItem = true;
				break;*/
			default:
				break;
			}

			return true;
		}
	}
}

void Simon::CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects)
{
	float simon_l, simon_t, simon_r, simon_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);



	for (UINT i = 0; i < listObjects->size(); i++)
	{
		Enemy * enemy = dynamic_cast<Enemy*>(listObjects->at(i));
		if (dynamic_cast<Zombie *>(enemy))
		{
			OutputDebugStringW(L"ABC");
		}
		if (enemy == NULL)
			continue;

		// Không cần xét vùng active nữa khi nó đang active / destroyed
		if (enemy->GetState() == ACTIVE)
			continue;

		float enemy_l, enemy_t, enemy_r, enemy_b;
		enemy->GetActiveBoundingBox(enemy_l, enemy_t, enemy_r, enemy_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, enemy_l, enemy_t, enemy_r, enemy_b) == true)
		{
			D3DXVECTOR2 enemyEntryPostion = enemy->GetEntryPosition();

			if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie * zombie = dynamic_cast<Zombie*>(enemy);

				if (zombie->GetState() == ZOMBIE_IDLE && zombie->IsActivate() == true)
					zombie->SetState(ZOMBIE_ACTIVE);
			}
			else if (dynamic_cast<Panther*>(enemy))
			{
				Panther * panther = dynamic_cast<Panther*>(enemy);
				if (panther->GetState() == PANTHER_IDLE_INACTIVE)
					panther->SetState(PANTHER_ACTIVE);
			}
			else if (dynamic_cast<Bat*>(enemy))
			{
				Bat * bat = dynamic_cast<Bat*>(enemy);

				if (bat->GetState() == BAT_IDLE && bat->IsActivate() == true)
					bat->SetState(BAT_ACTIVE);
			}
			else if (dynamic_cast<FishMan*>(enemy))
			{
				FishMan * fishman = dynamic_cast<FishMan*>(enemy);

				if (fishman->GetState() == FISHMAN_IDLE && fishman->IsActivate() == true)
					fishman->SetState(FISHMAN_ACTIVE);
			}
			//else if (dynamic_cast<Boss*>(enemy))
			//{
			//	Boss * boss = dynamic_cast<Boss*>(enemy);
			//	boss->SetState(BOSS_ACTIVE);
			//}
		}
	}
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
			vx = SIMON_WALKING_SPEED * 5;
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

void Simon::DoAutoWalk()
{
	SetState(SIMON_WALK);
	vx = SIMON_WALKING_SPEED - 0.05f;
}