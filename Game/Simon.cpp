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
	AddAnimation("simon_stand_stairup_ani");
	AddAnimation("simon_stand_stairdown_ani");
	
	whip = new Whip();
	hp = 16;
	life = 3;
	heart = 15;
	score = 0;
	start = false;
	isCoGround = false;
	MaxLevelWhip = false;
	isCheckReset = false;
	SubWeapon = -1;
	autoWalk = false;
	startTime = 0;
	isStandOnStair = false;
	isFinishDelayAnimation = true;
	SetState(SIMON_STAND_IDLE);
}



void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 

	GameObject::Update(dt);

	//score 
	if (isStandOnStair == false && autoWalk == false)
	{
			vy += GRAVITY * dt;
	}




	if (autoWalk == true)
		this->DoAutoWalk();



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	

	DelayAnimation();
	
	// turn off collision when die 
	if (state != SIMON_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed



	// No collision occured, proceed normally
	if (coEvents.size() == 0 && autoWalk != false)
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
			x += min_tx * dx + nx * 0.4f;
			if(ny <= 0)
				y += min_ty * dy + ny * 0.4f;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// collision of Simon and Candle -> do nothing -> update x;
			if (dynamic_cast<Ground *>(e->obj) || dynamic_cast<BreakWall*>(e->obj))
			{
				if (e->ny != 0)
				{			
					if (e->ny == -1.0f && (state != SIMON_DEFLECT || (state == SIMON_DEFLECT && vy > 0)))
					{
						vy = 0;
						isCoGround = true;
						isJumping = false;
						if (hp == 0)
						{
							SetState(SIMON_DIE);
							return;
						}
					}
					else
						y += dy;
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
				if (e->nx < 0)
				{
					x += dx;
				}
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
				OutputDebugString(L"co vao day ne");
				x += dx;
				y += dy;

				ChangeScene * obj = dynamic_cast<LPCHANGESCENE>(e->obj);
				changeScene = obj->GetIdScene();
			}
			else if (dynamic_cast<Water*>(e->obj))
			{
				Water * water = dynamic_cast<Water*>(e->obj);
				water->AddBubbles(x, y + SIMON_BBOX_HEIGHT);

				SetState(SIMON_DIE);
				isFallingWater = true;
				return;
			}
			else if (dynamic_cast<FireBall*>(e->obj))
			{
				if (untouchable == 0)
				{
					LoseHP(1);
					e->obj->isDestroy = true;
					if (isStandOnStair == false || hp == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1.0f;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1.0f;
						}
						SetState(SIMON_DEFLECT);
					}
					else
					{
						if (e->nx != 0) x += dx;
						if (e->ny > 0) y += dy;
					}
					StartUntouchable();
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj) || dynamic_cast<Panther*>(e->obj)
				|| dynamic_cast<Bat*>(e->obj) || dynamic_cast<FishMan*>(e->obj) || dynamic_cast<Boss*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (dynamic_cast<Zombie*>(e->obj))
					{
						Zombie * zombie = dynamic_cast<Zombie*>(e->obj);
						LoseHP(zombie->GetAttack());
					}
					else if (dynamic_cast<Panther*>(e->obj))
					{
						Panther * panther = dynamic_cast<Panther*>(e->obj);
						LoseHP(panther->GetAttack());
					}
					else if (dynamic_cast<Bat*>(e->obj))
					{
						Bat * bat = dynamic_cast<Bat*>(e->obj);
						bat->LoseHP(1);
						LoseHP(bat->GetAttack());
					}
					else if (dynamic_cast<FishMan*>(e->obj))
					{
						FishMan * fishman = dynamic_cast<FishMan*>(e->obj);
						LoseHP(fishman->GetAttack());
					}
					else if (dynamic_cast<Boss*>(e->obj))
					{
						Boss * boss = dynamic_cast<Boss*>(e->obj);
						LoseHP(boss->GetAttack());
					}

					if (isStandOnStair == false || hp == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1.0f;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1.0f;
						}
						SetState(SIMON_DEFLECT);
					}
					
					else
					{
						if (e->nx != 0) x += dx;
						if (e->ny > 0) y += dy;
					}
					StartUntouchable();
				}
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	whip->setNx(nx);
	whip->SetWhipPosition(x, y, isSit);
	if (isAttack() && !isHitSubWeapon)
	{

		if (animations[state]->getCurrentFrame() == animations[state]->getLastFrame()) // chỉ xét va chạm khi render tới sprite cuối cùng của roi
		{
			whip->Update(dt, coObjects);
		}
	}
}




void Simon::Render()
{
	if (isFallingWater)
		return;
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
			case ITEM_AXE:
			case ITEM_STOP_WATCH:
			case ITEM_HOLY_WATER:
			case ITEM_BOOMERANG:
				SubWeapon = idItem;
				break;
			case ITEM_SMALL_HEART:
				heart += 1;
				break;
			case ITEM_BIG_HEART:
				heart += 5;
				break;
			case ITEM_CROSS:
				isCoCross = true;
				break;
			case ITEM_WHIP:
				SetState(SIMON_EFFECT);
				vx = 0;
				whip->UpLevelWhip();
				if (whip->checkLevel())
					MaxLevelWhip = true;
				break;
			case ITEM_MONEY:
				score += 100;
				break;

			case ITEM_PORKCHOP:
				hp += 2;
				if (hp > 16)
					hp = 16;
				break;
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

				//OutputDebugStringW(L"GO AREA PANTHER");

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
				//fishman->setNx(tempNx);
			}
			else if (dynamic_cast<Boss*>(enemy))
			{
				Boss * boss = dynamic_cast<Boss*>(enemy);
				boss->SetState(BOSS_ACTIVE);
			}
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
		if(!isJumping)
			vx = 0;
		isSit = false;
		isFallingWater = false;
		isFinishDelayAnimation = true;
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
		isJumping = true;
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
	case SIMON_STAND_STAIR_UP:
	case SIMON_HIT_STAIR_DOWN:
	case SIMON_STAND_STAIR_DOWN:
		vx = 0;
		vy = 0;
		break;
	case SIMON_SIT_ATTACK:
	case SIMON_STAND_ATTACK:
		if (!isJumping)
		{
			vx = 0;
			vy = 0;
		}
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

	
	simon_t += 50;
	simon_b += 5;

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

			float upstair_x = -999, upstair_y = -999; 

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

void Simon::AutoWalk( float distance, int new_state, int new_nx)
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

void Simon::DelayAnimation()
{
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (state == SIMON_STAIR_UP)
	{
		if (start == false)
		{
			start = true;
			isFinishDelayAnimation = false;
			startTime += GetTickCount();
		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 200)
			{
				start = false;
				remain = 0;
				startTime = 0;
				isFinishDelayAnimation = true;
				reset();
				SetState(SIMON_STAND_STAIR_UP);
			}
		}
	}

	if (state == SIMON_DIE)
	{
		if (start == false)
		{
			start = true;
			isFinishDelayAnimation = false;
			startTime += GetTickCount();
		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 600)
			{
				start = false;
				remain = 0;
				startTime = 0;
				isFinishDelayAnimation = true;
				isCheckReset = true;
				OutputDebugString(L"co vao day");
			}
		}
	}

	if (state == SIMON_STAIR_DOWN)
	{
		if (start == false)
		{
			start = true;
			startTime += GetTickCount();
			isFinishDelayAnimation = false;
		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 200)
			{
				start = false;
				remain = 0;
				isFinishDelayAnimation = true;
				startTime = 0;
				reset();
				SetState(SIMON_STAND_STAIR_DOWN);
			}
		}
	}


	if (state == SIMON_DEFLECT)
	{
		if (start == false)
		{
			start = true;
			isFinishDelayAnimation = false;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > 600)
			{
				start = false;
				remain = 0;
				isFinishDelayAnimation = true;
				startTime = 0;
				reset();
				ResetAniamtionHit();
				whip->reset();
				SetState(SIMON_STAND_IDLE);


			}
		}
	}

	if (state == SIMON_EFFECT)
	{
		if (start == false)
		{
			isFinishDelayAnimation = false;
			start = true;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > SIMON_EFFECT_TIME)
			{
				start = false;
				isFinishDelayAnimation = true;
				remain = 0;
				startTime = 0;
				reset();
				SetState(SIMON_STAND_IDLE);
				
			}
		}
	}

	if (isAttack())
	{
		if (start == false)
		{
			start = true;
			isFinishDelayAnimation = false;
			startTime += GetTickCount();

		}
		else
		{
			float remain = GetTickCount() - startTime;

			if (remain > (DWORD)SIMON_ATTACK_TIME)
			{
				isFinishDelayAnimation = true;
				start = false;
				remain = 0;
				startTime = 0;
				reset();
				isHitSubWeapon = false;
				
				if (isStandOnStair == true)
				{
					if (nx > 0)
						if (stairDirection == 1)
							SetState(SIMON_STAND_STAIR_UP);
						else
							SetState(SIMON_STAND_STAIR_DOWN);
					else
						if (stairDirection == 1)
							SetState(SIMON_STAND_STAIR_DOWN); 
						else
							SetState(SIMON_STAND_STAIR_UP);
				}
				else
				{
					if (isSit)
						SetState(SIMON_SIT);
					else
						SetState(SIMON_STAND_IDLE);
				}
				whip->reset();
			}
		}
	}
}

void Simon::ResetAniamtionHit()
{
	animations[SIMON_SIT_ATTACK]->Reset();
	animations[SIMON_STAND_ATTACK]->Reset();
	animations[SIMON_HIT_STAIR_UP]->Reset();
	animations[SIMON_HIT_STAIR_DOWN]->Reset();
}

