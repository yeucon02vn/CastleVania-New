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
	AddAnimation("simon_stairup_ani");
	AddAnimation("simon_stairdown_ani");
	AddAnimation("simon_hitstairup_ani");
	AddAnimation("simon_hitstairdown_ani");
	AddAnimation("simon_deflect_ani"); 
	AddAnimation("simon_dead_ani");

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
	isStandOnStair = false;
}



void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 

	if (isStandOnStair == false && autoWalk == false)
	{
		vy += GRAVITY * dt;
	}
	GameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (autoWalk)
		DoAutoWalk();


	if (state == SIMON_STAIR_UP)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 250)
			{
				start = false;
				remain = 0;
				startTime = 0;
				//SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

	if (state == SIMON_STAIR_DOWN)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 250)
			{
				start = false;
				remain = 0;
				startTime = 0;
				//SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

	if (state == SIMON_HIT_STAIR_UP)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 350)
			{
				start = false;
				remain = 0;
				startTime = 0;
				//SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

	if (state == SIMON_HIT_STAIR_DOWN)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 350)
			{
				start = false;
				remain = 0;
				startTime = 0;
				//SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

	if (state == SIMON_DEFLECT)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 500)
			{
				start = false;
				remain = 0;
				startTime = 0;
				SetState(SIMON_STAND_IDLE);
				reset();
			}
		}
	}

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

		if (autoWalk == false)
		{
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// collision of Simon and Candle -> do nothing -> update x;
			if (dynamic_cast<Ground *>(e->obj)  )
			{
				
				if (autoWalk )
				{
					x += dx;
				}
				if (e->ny != 0)
				{					
					if (e->ny == -1.0f && (state != SIMON_DEFLECT || (state == SIMON_DEFLECT && vy > 0)))
					{
						vy = 0;
						isCoGround = true;
					}
					else
						y += dy;
				}	

			}
			else if (dynamic_cast<BreakWall*>(e->obj))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1.0f && (state != SIMON_DEFLECT || (state == SIMON_DEFLECT && vy > 0)))
					{
						vy = 0;
						isCoGround = true;
					}
					else
						y += dy;
				}
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			else if (dynamic_cast<Candle *>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
				{
					y += dy;
					x += dx;
				}
			}
			else if (dynamic_cast<Items *>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
				{
					y += dy;
					x += dx;
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
					SetState(SIMON_WALK);
					vx = SIMON_WALKING_LOWER_SPEED;
					vy = 0;
					AutoWalk(80, SIMON_STAND_IDLE, nx);
				}
			}
			else if (dynamic_cast<LPCHANGESCENE>(e->obj))
			{
				x += dx;
				y += dy;

				ChangeScene * obj = dynamic_cast<LPCHANGESCENE>(e->obj);
				changeScene = obj->GetIdScene();
			}
			//else if (dynamic_cast<Water*>(e->obj))
			//{
			//	Water * water = dynamic_cast<Water*>(e->obj);
			//	water->AddBubbles(x, y + SIMON_BBOX_HEIGHT);

			//	SetState(SIMON_DIE);
			//	//isFallingWater = true;
			//	return;
			//}
			//else if (dynamic_cast<FireBall*>(e->obj))
			//{
			//	//LoseHP(1);
			//	e->obj->isDestroy = true;
			//}
			else if (dynamic_cast<Zombie*>(e->obj) || dynamic_cast<Panther*>(e->obj)
				|| dynamic_cast<Bat*>(e->obj) || dynamic_cast<FishMan*>(e->obj))
			{
				/*if (state != SIMON_EFFECT && untouchableTimer->IsTimeUp() == true && invisibilityTimer->IsTimeUp() == true)
				{*/
					//untouchableTimer->Start();
				if (untouchable == 0)
				{
					if (dynamic_cast<Zombie*>(e->obj))
					{
						Zombie * zombie = dynamic_cast<Zombie*>(e->obj);
						//LoseHP(zombie->GetAttack());
					}
					else if (dynamic_cast<Panther*>(e->obj))
					{
						Panther * panther = dynamic_cast<Panther*>(e->obj);
						//LoseHP(leopard->GetAttack());
					}
					else if (dynamic_cast<Bat*>(e->obj))
					{
						Bat * bat = dynamic_cast<Bat*>(e->obj);
						bat->LoseHP(1);
						//LoseHP(bat->GetAttack());
					}
					else if (dynamic_cast<FishMan*>(e->obj))
					{
						FishMan * fishman = dynamic_cast<FishMan*>(e->obj);
						//LoseHP(fishman->GetAttack());
					}

					if (isStandOnStair == false || hp == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1.0f;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1.0f;
						}
						StartUntouchable();
						SetState(SIMON_DEFLECT);
					}

					else
					{
						if (e->nx != 0) x += dx;
						if (e->ny != 0) y += dy;
					}
				}
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
				SubWeapon = WEAPON_KNIFE;
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

void Simon::LoseHP(int x)
{
	{
		hp -= x;

		if (hp <= 0)
		{
			hp = 0;
			SetState(SIMON_DIE);
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
			int tempNx = x < enemy->x ? -1 : 1;
			if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie * zombie = dynamic_cast<Zombie*>(enemy);

				if (zombie->GetState() == ZOMBIE_IDLE && zombie->IsActivate() == true)
					zombie->SetState(ZOMBIE_ACTIVE);
				zombie->setNx(tempNx);
			}
			else if (dynamic_cast<Panther*>(enemy))
			{

				OutputDebugStringW(L"GO AREA PANTHER");

				Panther * panther = dynamic_cast<Panther*>(enemy);
				if (panther->GetState() == PANTHER_IDLE)
					panther->SetState(PANTHER_ACTIVE);
				panther->setNx(tempNx);
			}
			else if (dynamic_cast<Bat*>(enemy))
			{
				Bat * bat = dynamic_cast<Bat*>(enemy);

				if (bat->GetState() == BAT_IDLE && bat->IsActivate() == true)
					bat->SetState(BAT_ACTIVE);
				bat->setNx(tempNx);
			}
			else if (dynamic_cast<FishMan*>(enemy))
			{
				FishMan * fishman = dynamic_cast<FishMan*>(enemy);

				if (fishman->GetState() == FISHMAN_IDLE && fishman->IsActivate() == true)
					fishman->SetState(FISHMAN_ACTIVE);
				fishman->setNx(tempNx);
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
		isStandOnStair = false;
		break;
	case SIMON_WALK:
		isCoGround = true;
		isStandOnStair = false;
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;
		break;
	case SIMON_JUMP:
		isCoGround = false;
		isStandOnStair = false;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_SIT:
		isSit = true;
		isStandOnStair = false;
		vx = 0;
		vy = 0;
		break;
	case SIMON_EFFECT:
		vx = 0;
		vy = 0;
		isStandOnStair = false;
		break;
	case SIMON_STAIR_UP:
		if (nx > 0) vx = SIMON_STAIR_SPEED;
		else vx = -SIMON_STAIR_SPEED;
		vy = -SIMON_STAIR_SPEED;
		isStandOnStair = true;
		break;
	case SIMON_STAIR_DOWN:
		if (nx > 0) vx = SIMON_STAIR_SPEED;
		else vx = -SIMON_STAIR_SPEED;
		vy = SIMON_STAIR_SPEED;
		isStandOnStair = true;
		break;
	case SIMON_DEFLECT:
		vy = -SIMON_DEFLECT_SPEED_Y;
		if (nx > 0) vx = -SIMON_DEFLECT_SPEED_X;
		else vx = SIMON_DEFLECT_SPEED_X;
		break;
	case SIMON_HIT_STAIR_UP:
	case SIMON_HIT_STAIR_DOWN:
		vx = 0;
		vy = 0;
		break;
	case SIMON_DIE:
		vx = 0;
		vy = 0;
		life -= 1;
		break;
	default:
		break;
	}


}


bool Simon::isAttack()
{
	return state == SIMON_SIT_ATTACK || state == SIMON_STAND_ATTACK || state == SIMON_HIT_STAIR_UP || state == SIMON_HIT_STAIR_DOWN;
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 15;
	top = y;
	right = x + SIMON_BBOX_WIDTH + 2;
	bottom = y + SIMON_BBOX_HEIGHT;

}

bool Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listCheckStair)
{
	canMoveUpStair = false;
	canMoveDownStair = false;


	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_t += 50;
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống

	for (UINT i = 0; i < listCheckStair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		listCheckStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);


		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			if (listCheckStair->at(i)->GetState() == 0) stairDirection = 1;
			else stairDirection = -1;

			listCheckStair->at(i)->GetPosition(goToStair.x, goToStair.y);

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			if (simon_b < stair_b) canMoveDownStair = true;
			if (y >= stair_t - 35) canMoveUpStair = true;

			// kiểm tra xem simon có thể di chuyển lên/xuống hay ko
			// (dựa vào toạ độ của 2 bậc liền kề hơn/kém nhau 32)

			float upstair_x = -999, upstair_y = -999; // toạ độ của bậc thang liền kề

			for (UINT j = 0; j < listCheckStair->size(); j++)
			{
				if (i == j)
					continue;

				listCheckStair->at(j)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_WIDTH && dy == -GROUND_HEIGHT) // vì bậc nằm trên nên dy = -...
					canMoveUpStair = true;

				if (dx == GROUND_WIDTH && dy == GROUND_HEIGHT) // vì bậc nằm duoi nên dy = +...
					canMoveDownStair = true;
			}


			return true; // collision between Simon and stairs
		}
	}
}



void Simon::reset()
{
	animations[state]->Reset();
}

void Simon::AutoWalk(float distance, int new_state, int new_nx)
{
	autoWalk = true;

	autoWalkDistance = distance;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}

void Simon::DoAutoWalk()
{
	if (abs(dx) <= abs(autoWalkDistance))
	{
		x += dx;
		y += dy;
		autoWalkDistance -= dx;
	}
	else
	{
		x += autoWalkDistance;
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;

		SetState(state);
		if (state == SIMON_STAIR_DOWN) y += 1.0f; // + 1.0f để đảm bảo simon sẽ va chạm với bậc thang trong lần update kế tiếp

		autoWalk = false;
		autoWalkDistance = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
}