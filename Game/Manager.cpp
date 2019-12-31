#include "Manager.h"

Manager::Manager(Game * game)
{
	this->game = game;

	// INIT
	simon = new Simon();
	boss = new Boss();
	mapsObjects = MapsObjectsManager::GetInstance();
	ui = new UI();
	ui->SetSimon(simon);
	ui->SetDevice(game->GetDirect3DDevice());
	ui->SetFont(game->GetFont());
	canControl = true;
	
	weapon = new SubWeapon();
	weapon->SetDestroy(true);
	tileMap = new TileMap(FRAME_TILE_MAP, FRAME_TILE_MAP);
	tileMap->LoadResource();
	Init(GAMESTATE1);
}

Manager::~Manager()
{
}

void Manager::Init(int idScene)
{
	this->idScene = idScene;
	LoadObjects(idScene);
	SetGameState(idScene);
	ui->ResetTime();
	
}

void Manager::LoadObjects(int id)
{

	vector<Object> objects = mapsObjects->Get(id)->getMapobject();


	for(auto obj : objects)
	switch (obj.idObject)
	{
	case GRID:
		grid = Grid::GetInstance(true,obj.x,obj.y);
		break;
	case CANDLE:
	{
		Candle * candle = new Candle();
		candle->SetPosition(obj.x, obj.y);
		candle->SetState(obj.state);
		candle->isDropItem = true;
		grid->Add(candle, obj.x / 256, obj.y /240);
		break;
	}
	case GROUND:
	{
		Ground * ground = new Ground();
		ground->SetPosition(obj.x, obj.y);
		ground->SetState(obj.state);

		grid->Add(ground, obj.x / 256, obj.y / 240);
		break;
	}

	case STAIR:
	{
		Stair * stair = new Stair();
		stair->SetPosition(obj.x, obj.y);
		stair->SetState(obj.state);
		grid->Add(stair, obj.x / 256, obj.y / 240);
		break;
	}
	case DOOR:
	{
		Door * door = new Door();
		door->SetPosition(obj.x, obj.y);
		door->SetState(obj.state);
		grid->Add(door, obj.x / 256, obj.y / 240);
		break;
	}
	case ZOMBIE:
	{
		Zombie * zombie = new Zombie();
		zombie->SetEntryPosition(obj.x, obj.y);
		zombie->SetState(ZOMBIE_IDLE);
		grid->Add(zombie, obj.x / 256, obj.y / 240);
		break;
	}
	case PANTHER:
	{
		Panther * panther = new Panther();
		panther->SetEntryPosition(obj.x, obj.y);
		panther->SetState(PANTHER_IDLE_INACTIVE);
		grid->Add(panther, obj.x / 256, obj.y / 240);
		break;
	}
	case BAT:
	{
		Bat * bat = new Bat();
		bat->SetEntryPosition(obj.x, obj.y);
		bat->SetState(BAT_IDLE);
		grid->Add(bat, obj.x / 256, obj.y / 240);
		break;
	}
	case FISHMAN:
	{
		FishMan * fishman = new FishMan();
		fishman->SetEntryPosition(obj.x, obj.y);
		fishman->SetState(FISHMAN_IDLE);
		grid->Add(fishman, obj.x / 256, obj.y / 240);
		break;
	}
	case CHANGE_SCENE_OBJECT:
	{
		ChangeScene * changeScene = new ChangeScene();
		changeScene->SetPosition(obj.x, obj.y);
		changeScene->SetIdScene(obj.state);
		grid->Add(changeScene, obj.x / 256, obj.y / 240);
		break;
	}
	case WATER:
	{
		water = new Water();
		water->SetPosition(obj.x, obj.y);
		grid->Add(water, obj.x / 256, obj.y / 240);
		break;
	}
	case BREAKWALL:
	{
		BreakWall * breakwall = new BreakWall();
		breakwall->SetPosition(obj.x, obj.y);
		breakwall->SetState(HIDDEN);
		if(!(obj.x == 3584 && obj.y == 384))
			breakwall->isDropItem = true;
		grid->Add(breakwall, obj.x / 256, obj.y /240);
		break;
	}

	case BOSS:
	{
		boss = new Boss();
		boss->setPoisitionBoss(obj.x, obj.y);
		grid->Add(boss, obj.x / 256, obj.y / 240);
		break;
	}

	default:
		break;
	}
}

void Manager::GetObjectFromGrid()
{
	listGetObject.clear();
	listGridObjects.clear();
	listStairs.clear();
	listDoors.clear();
	listStaticObjects.clear();
	listMovingObjects.clear();

	grid->Get(game->getCamPosition(), listGetObject);

	//DebugOut(L"%d \n", listUnits.size());

	for (UINT i = 0; i < listGetObject.size(); i++)
	{
		LPGAMEOBJECT obj = listGetObject[i];
		listGridObjects.push_back(obj);
		if (dynamic_cast<Stair*>(obj))
			listStairs.push_back(obj);
		else if (dynamic_cast<Door*>(obj))
			listDoors.push_back(obj);
		else if (dynamic_cast<Candle*>(obj) ||
			dynamic_cast<Water*>(obj) || dynamic_cast<BreakWall*>(obj) || dynamic_cast<Ground*>(obj))
			listStaticObjects.push_back(obj);
		else
			listMovingObjects.push_back(obj);
	}
}


void Manager::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Items*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Ground*>(obj) || (dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Zombie*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Ground*>(obj) || (dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Panther*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Ground*>(obj) || (dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<FishMan*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if ((dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN) ||
				dynamic_cast<Ground*>(obj) || dynamic_cast<Water*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<SubWeapon*>(curObj))
	{
		SubWeapon * weapon = dynamic_cast<SubWeapon*>(curObj);

		coObjects.push_back(simon); // dùng để xét va chạm của Simon với boomerang
		if (isBossFighting == true && boss->GetState() == BOSS_ACTIVE)
			coObjects.push_back(boss);

		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Ground*>(obj) )
				coObjects.push_back(obj);
			else if (dynamic_cast<Candle*>(obj) || dynamic_cast<FireBall*>(obj))
				coObjects.push_back(obj);
			else if ((dynamic_cast<Zombie*>(obj) || dynamic_cast<Bat*>(obj))
				&& obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
			else if (dynamic_cast<Panther*>(obj) &&
				(obj->GetState() != PANTHER_IDLE_INACTIVE))
				coObjects.push_back(obj);
			else if (dynamic_cast<FishMan*>(obj) &&
				(obj->GetState() != FISHMAN_IDLE))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Simon*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Door*>(obj) || dynamic_cast<Ground*>(obj) ||
				dynamic_cast<ChangeScene*>(obj) || dynamic_cast<Water*>(obj) || dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);
			else if (dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN)
				coObjects.push_back(obj);
			else if (simon->autoWalk == false) // nếu simon auto-walk sẽ không xét va chạm với enemy
			{
				if (dynamic_cast<FireBall*>(obj) && obj->isDestroy == false)
					coObjects.push_back(obj);
				else if ((dynamic_cast<Zombie*>(obj) || dynamic_cast<Panther*>(obj) ||
					dynamic_cast<Bat*>(obj) ) && obj->GetState() == ACTIVE)
					coObjects.push_back(obj);
				else if (dynamic_cast<FishMan*>(obj) && (obj->GetState() == ACTIVE || obj->GetState() == FISHMAN_JUMP))
					coObjects.push_back(obj);
				else if (dynamic_cast<Boss*>(obj) && obj->GetState() != BOSS_SLEEP)
					coObjects.push_back(obj);
			}
		}
		for(auto obj : listItems)
			coObjects.push_back(obj);
	}
}


void Manager::Update(DWORD dt)
{
	if (SimonWalkThroughDoor() == true)
		return;

	if (canControl)
		Control();

	fnChangeScene();



	vector<LPGAMEOBJECT> listCoObjects;

	GetObjectFromGrid();
	CrossEffect();
	SetEnemiesSpawnPositon();

	GetColliableObjects(simon, listCoObjects);
	simon->Update(dt, &listCoObjects);
	simon->CheckCollisionWithItem(&listItems);
	simon->CheckCollisionWithEnemyActiveArea(&listGridObjects);


	for (int i = 0; i < listGridObjects.size(); i++)
	{
		LPGAMEOBJECT object = listGridObjects[i];
		listCoObjects.clear();
		GetColliableObjects(object, listCoObjects);

		if (listGridObjects[i]->isDestroy == false)
		{
			listGridObjects[i]->Update(dt, &listCoObjects);
			if (dynamic_cast<FishMan*>(listGridObjects[i]))
			{
				FishMan * fishman = dynamic_cast<FishMan*>(listGridObjects[i]);

				if (fishman->CanHit() == true)
				{
					fishman->SetState(FISHMAN_HIT);

					// Đặt hướng quay mặt của Fishman sau khi bắn (quay về phía simon)
					int new_nx;
					if (fishman->x < simon->x) new_nx = 1;
					else new_nx = -1;

					fishman->Hit(grid, new_nx);
				}
			}
			else if (dynamic_cast<Boss*>(object))
			{
				// Passing simon's position for boss's movement
				boss->AimSimon(simon, dt);
			}
		}
		else
		{
			DeleteObject(listGridObjects[i], i);
		}
	}

	for (int i = 0; i < listEffects.size(); i++)
	{
		if(listEffects[i]->isDestroy != true)
			listEffects[i]->Update(dt);
		else
			listEffects.erase(listEffects.begin() + i);
	}
	for (int i = 0; i < listItems.size(); i++)
	{
		LPGAMEOBJECT object = listItems[i];
		listCoObjects.clear();
		GetColliableObjects(object, listCoObjects);
		if (listItems[i]->isDestroy != true)
			listItems[i]->Update(dt, &listCoObjects);
		else
			listItems.erase(listItems.begin() + i);
	}


	if (weapon->isDestroy != true)
	{
		listCoObjects.clear();
		GetColliableObjects(weapon, listCoObjects);
		weapon->Update(dt, &listCoObjects);
	}
	// update vị trí của quái vào cell mới
	

	if (isBossFighting == true && simon->x < game->getCamPosition().x)
		simon->x = game->getCamPosition().x;
	
	SetInactivationByPosition();
	UpdateCam();
	grid->Move(game->getCamPosition());
	ui->Update(boss->GetHP(), dt, idScene);
	ResetGame();
}

void Manager::UpdateCam()
{
	if (isBossFighting == true)				// Boss fight -> not moving camera
		return;

	if (boss->GetState() == BOSS_ACTIVE)
	{
		isBossFighting = true;
		return;
	}

	if (simon->x >= tileMap->getMinWidthMap() + SCREEN_WIDTH / 2 && simon->x < tileMap->getMaxWidthMap() - SCREEN_WIDTH / 2)
	{
		simon->getX(pos.x);
		pos.x -= SCREEN_WIDTH / 2;
		game->setCamPosition(pos.x, 0);
	}
}

void Manager::Render()
{
	tileMap->DrawMap(game->getCamPosition());
	ui->Render();
	
	for (int i = 0; i < listStaticObjects.size(); i++)
	{		
		listStaticObjects[i]->Render();
	}
	for (int i = 0; i < listStairs.size(); i++)
	{
		listStairs[i]->Render();
	}
	for (int i = 0; i < listMovingObjects.size(); i++)
	{
		listMovingObjects[i]->Render();
	}

	if (listEffects.size() != 0)
	{
		for (int i = 0; i < listEffects.size(); i++)
		{
			if(listEffects[i]->isDestroy == false)
				listEffects[i]->Render();
		}
	}
	simon->Render();

	
	for (int i = 0; i < listItems.size(); i++)
	{
		listItems[i]->Render();
	}

	for (int i = 0; i < listDoors.size(); i++)
	{
		listDoors[i]->Render();
	}
	weapon->Render();
}

//void Manager::UpdateGrid()
//{
//	GetObjectFromGrid();
//	for (int i = 0; i < listGetObject.size(); i++)
//	{
//		LPGAMEOBJECT obj = listGetObject[i];
//		if (dynamic_cast<Enemy*>(obj))
//		{
//			float newPos_x, newPos_y;
//			obj->GetPosition(newPos_x, newPos_y);
//			listGetObject[i]->Move(newPos_x, newPos_y);
//		}
//	}
//}

void Manager::DeleteObject(LPGAMEOBJECT object, int i)
{
	if (object->isDropItem == true)
	{
		Items *item = new Items();
		if (simon->isMaxLevelWhip())
			item->isMax();
		item->Random();
		item->isEffect = false;
		item->SetPosition(object->x, object->y);
		listItems.push_back(item);
		
	}
	if (dynamic_cast<BreakWall*>(object))
	{
		object->isDestroy = false;
		return;
	}

	if (object->isEffect == false)
	{
		EffectDead *effect = new EffectDead();
		effect->SetState(EFFECTDEAD_STAR);
		effect->SetPosition(object->x, object->y);
		effect->isEffect = true;
		listEffects.push_back(effect);

		effect = new EffectDead();
		effect->SetState(EFFECTDEAD_FIRE);
		effect->isEffect = true;
		effect->SetPosition(object->x, object->y);
		listEffects.push_back(effect);
		if (dynamic_cast<Enemy*>(object))
		{
			object->SetState(INACTIVE);
		}
	}
	if (dynamic_cast<Enemy*>(object))
	{
		if(dynamic_cast<Boss*>(object))
			grid->Delete(game->getCamPosition());
		return;
	}
	else
	{
		grid->Delete(game->getCamPosition());
	}
}

void Manager::SetGameState(int gameState)
{
	switch (gameState)
	{
	case SCENE1:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(0, 302);
		game->setCamPosition(0, 0);
		tileMap->LoadMap(SCENE1);
		break;
	case SCENE2:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(0, 335);
		game->setCamPosition(0, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE2);
		break;
	case SCENE2_1:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(3115, 143);
		game->setCamPosition(3056, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE2_1);
		break;
	case SCENE2_2:
		simon->SetState(SIMON_STAIR_UP);
		simon->SetPosition(3172, 420);
		game->setCamPosition(3056, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE2_2);
		break;
	case SCENE2_3:
		simon->SetState(SIMON_STAIR_UP);
		simon->SetPosition(3812, 420);
		game->setCamPosition(3450, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE2_3);
		break;
	case SCENE2_BOSS:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(4139, 143);
		game->setCamPosition(4079, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE2_BOSS);
		break;
	case SCENE3_1:
		simon->SetState(SIMON_STAIR_DOWN);
		simon->SetPosition(100, 98);
		simon->setNx(1);
		game->setCamPosition(0, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE3_1);
		break;
	case SCENE3_2:
		simon->SetState(SIMON_STAIR_DOWN);
		simon->SetPosition(737, 93);
		simon->setNx(1);
		game->setCamPosition(383, 0);
		simon->autoWalk = false;
		tileMap->LoadMap(SCENE3_2);
		break;
	default:
		break;
	}
}


bool Manager::IsInViewport(LPGAMEOBJECT object)
{
	D3DXVECTOR2 camPosition = game->getCamPosition();

	float obj_x, obj_y;
	object->GetPosition(obj_x, obj_y);

	return obj_x >= camPosition.x && obj_x < camPosition.x + SCREEN_WIDTH
		&& obj_y >= camPosition.y && obj_y < camPosition.y + SCREEN_HEIGHT;
}

void Manager::SetInactivationByPosition()
{

	for (auto object : listGridObjects)
	{
		if (IsInViewport(object) == false)
		{
			if (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(object);
				zombie->SetState(ZOMBIE_IDLE);
			}
			else if (dynamic_cast<Panther*>(object) && object->GetState() == PANTHER_ACTIVE)
			{
				auto leopard = dynamic_cast<Panther*>(object);
				leopard->SetState(PANTHER_IDLE_INACTIVE);
			}
			else if (dynamic_cast<Bat*>(object) && object->GetState() == BAT_ACTIVE)
			{
				auto bat = dynamic_cast<Bat*>(object);
				bat->SetState(BAT_IDLE);
			}
			else if (dynamic_cast<FishMan*>(object)
				&& (object->GetState() == FISHMAN_ACTIVE || object->GetState() == FISHMAN_JUMP))
			{
				auto fishman = dynamic_cast<FishMan*>(object);
				fishman->SetState(FISHMAN_IDLE);
			}
			else if (dynamic_cast<FireBall*>(object) && object->isDestroy == false)
				object->isDestroy = true;
			else if (dynamic_cast<Items*>(object) && object->isDestroy == false)
				object->isDestroy = true;
		}
	}

	if (IsInViewport(weapon) == false)
		weapon->isDestroy = true;

}

void Manager::SetEnemiesSpawnPositon()
{
	if (simon->isCoCross == true)
		return;
	for (auto obj : listGridObjects)
	{
		if (dynamic_cast<Zombie*>(obj))
		{
			Zombie * zombie = dynamic_cast<Zombie*>(obj);

			if (zombie->GetState() != ZOMBIE_IDLE && zombie->isFinishRespawn == false)
			{
				zombie->isFinishRespawn = true;

				float simon_x, simon_y;
				simon->GetPosition(simon_x, simon_y);

				int nx = zombie->GetEntryPosition().x < simon_x ? 1 : -1;
				zombie->setNx(nx);

				// Cần random một khoảng nhỏ để tránh việc các zombie spawn cùng lúc, tại cùng một vị trí
				int randomDistance = rand() % 20;

				float x, y;
				y = zombie->GetEntryPosition().y;
				if (nx == -1)
					x = game->getCamPosition().x + SCREEN_WIDTH - (ENEMY_DEFAULT_BBOX + randomDistance);
				else
					x = game->getCamPosition().x + (ENEMY_DEFAULT_BBOX + randomDistance);

				zombie->SetPosition(x, y);
				zombie->SetState(ZOMBIE_ACTIVE);
			}
		}
		else if (dynamic_cast<Panther*>(obj))
		{
			Panther * panther = dynamic_cast<Panther*>(obj);

			if (panther->GetState() == PANTHER_IDLE_INACTIVE)
			{
				if (panther->IsActivate() == true && IsInViewport(panther) == true
					&& abs(simon->x - panther->GetEntryPosition().x) > 200)
				{
					int nx = panther->GetEntryPosition().x < simon->x ? 1 : -1;
					panther->setNx(nx);

					panther->SetState(PANTHER_IDLE);
				}
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat * bat = dynamic_cast<Bat*>(obj);

			if (bat->GetState() != BAT_IDLE && bat->isFinishRespawn == false)
			{
				bat->isFinishRespawn = true;

				// set random hướng cho dơi
				int listNx[2] = { -1, 1 };
				int rndIndex = rand() % 2;
				bat->setNx(listNx[rndIndex]);

				// set vị trí cho dơi
				// dơi bay ngang tầm simon, từ phía cuối của 2 đầu màn hình)
				float bx, by;
				int randomDistance = rand() % 30;

				by = simon->y + (rand() % SIMON_BBOX_HEIGHT);

				if (bat->nx == -1)
					bx = game->getCamPosition().x + SCREEN_WIDTH - (ENEMY_DEFAULT_BBOX - randomDistance);
				else
					bx = game->getCamPosition().x + (ENEMY_DEFAULT_BBOX - randomDistance);

				bat->SetPosition(bx, by);
				bat->SetState(BAT_ACTIVE);
			}
		}
		else if (dynamic_cast<FishMan*>(obj))
		{
			FishMan * fishman = dynamic_cast<FishMan*>(obj);

			if (fishman->GetState() != FISHMAN_IDLE && fishman->isFinishRespawn == false)
			{
				fishman->isFinishRespawn = true;

				// Set vị trí cho fishman dựa vào vị trí của Simon
				float simon_x, simon_y;
				simon->GetPosition(simon_x, simon_y);

				int nx = simon_x > fishman->GetEntryPosition().x ? 1 : -1;
				float distance = 50 + rand() % 150;

				float x = simon_x - nx * distance;
				float y = fishman->GetEntryPosition().y;

				fishman->setNx(nx);
				fishman->SetPosition(x, y);

				fishman->SetState(FISHMAN_JUMP);

				// Thêm bubbles vào water để render bọt nước
				water->AddBubbles(x, y);
			}
		}
	}
}


void Manager::Control()
{
	if (simon->isAttack() || simon->GetState() == SIMON_EFFECT || simon->autoWalk == true || simon->GetState() == SIMON_DEFLECT || simon->GetState() == SIMON_DIE || simon->isFinishDelayAnimation == false)
		return;

	if (IsKeyPress(DIK_SPACE))
	{
		if (simon->isJumping == true || simon->GetState() == SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
	}

	else if (IsKeyDown(DIK_UP))
	{
		
		if (CheckSimonCollisionStair() == true)
		{
			if (simon->isJumping)
				return;
			Simon_Stair_Up();
			return;
		}

		if (IsKeyPress(DIK_Z))
		{
			if (simon->GetSubWeapon() == -1)
			{
				return;
			}
			if (simon->GetHeart() <= 0)
				return;
			if (weapon->isDestroy == false)
				return;

			float x, y;
			simon->GetPosition(x, y);

			if (simon->GetState() == SIMON_SIT) y += 25.0f; // khớp vị trí tay
			else y += 10.0f;
			if (simon->nx < 0) x += 30.0f;

			weapon->SetPosition(x, y);

			// nx
			weapon->isDestroy = false;
			weapon->setNx(simon->nx);
			weapon->SetState(simon->SubWeapon);
			simon->isHitSubWeapon = true;
			simon->LoseHeart(1);
			if (simon->GetState() == SIMON_SIT)
				simon->SetState(SIMON_SIT_ATTACK);
			else
				simon->SetState(SIMON_STAND_ATTACK);
		}
	}
	else if (IsKeyPress(DIK_Z))
	{
		if (CheckSimonCollisionStair() && simon->isStandOnStair == true)
		{
			if (simon->nx > 0)
				if (simon->stairDirection == 1)
					simon->SetState(SIMON_HIT_STAIR_UP);
				else
					simon->SetState(SIMON_HIT_STAIR_DOWN);
			else
				if (simon->stairDirection == 1)
					simon->SetState(SIMON_HIT_STAIR_DOWN);
				else
					simon->SetState(SIMON_HIT_STAIR_UP);
		}
		else if (simon->GetState() == SIMON_SIT)
			simon->SetState(SIMON_SIT_ATTACK);
		else //if (simon->GetState() == SIMON_STAND_IDLE || simon->GetState() == SIMON_JUMP)
			simon->SetState(SIMON_STAND_ATTACK);

	}
	else if (IsKeyDown(DIK_LEFT))
	{
		if(CheckSimonCollisionStair() == true && simon->isStandOnStair == true)
		{
			if (simon->stairDirection == 1) // cầu thang trái dưới - phải trên
				Simon_Stair_Down();
			else
				Simon_Stair_Up();

			return;
		}
		if (simon->isCoGround == false || simon->isJumping) // 
			return;
		simon->SetState(SIMON_WALK);
		simon->setNx(-1);
	}
	else if (IsKeyDown(DIK_RIGHT))
	{
		if (CheckSimonCollisionStair() == true && simon->isStandOnStair == true)
		{
			if (simon->stairDirection == 1) // cầu thang trái dưới - phải trên
				Simon_Stair_Up();
			else
				Simon_Stair_Down();

			return;
		}
		if (simon->isCoGround == false || simon->isJumping) // || simon->isJumping
			return;
		simon->SetState(SIMON_WALK);
		simon->setNx(1);
	}
	else if (IsKeyDown(DIK_DOWN))
	{
		if (CheckSimonCollisionStair() == true)
		{
			if (simon->isJumping)
				return;
			Simon_Stair_Down();
			return;
		}
		if (simon->isCoGround == false || simon->isJumping)
			return;
		simon->SetState(SIMON_SIT);
	}
	else
	{
		if (CheckSimonCollisionStair())
		{
			if (Simon_Stair_Stand() == true)
				return;
		}

		if (simon->isCoGround == false || simon->isAttack())
			return;
		simon->SetState(SIMON_STAND_IDLE);
	}
	// dev test
#pragma region Phím xử lý khác
	if (IsKeyPress(DIK_K))
	{
		simon->SetState(SIMON_DIE);
	}
	else if (IsKeyPress(DIK_L))
	{
		simon->AddHP(2);

	}
#pragma endregion

#pragma region Phím nhảy qua map Q W E R T
	if (IsKeyPress(DIK_Q))
	{
		Init(GAMESTATE1);
	}
	else if (IsKeyPress(DIK_W))
	{
		Init(GAMESTATE2);
	}
	else if (IsKeyPress(DIK_E))
	{
		Init(GAMESTATE2);
		SetGameState(SCENE2_1);
	}
	else if (IsKeyPress(DIK_R))
	{
		Init(GAMESTATE3);
	}
	else if (IsKeyPress(DIK_T))
	{
		Init(GAMESTATE2);
		SetGameState(SCENE2_BOSS);
	}

#pragma endregion
#pragma region Phím item
	if (IsKeyPress(DIK_0))
	{
		Items *item = new Items();
		item->SetState(ITEM_SMALL_HEART);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_1))
	{
		Items *item = new Items();
		item->SetState(ITEM_BIG_HEART);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_2))
	{
		Items *item = new Items();
		item->SetState(ITEM_WHIP);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_3))
	{
		Items *item = new Items();
		item->SetState(ITEM_KNIFE);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_4))
	{
		Items *item = new Items();
		item->SetState(ITEM_BOOMERANG);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_5))
	{
		Items *item = new Items();
		item->SetState(ITEM_AXE);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_6))
	{
		Items *item = new Items();
		item->SetState(ITEM_STOP_WATCH);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_7))
	{
		Items *item = new Items();
		item->SetState(ITEM_HOLY_WATER);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_8))
	{
		Items *item = new Items();
		item->SetState(ITEM_CROSS);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
	else if (IsKeyPress(DIK_9))
	{
		Items *item = new Items();
		item->SetState(ITEM_PORKCHOP);
		item->isEffect = false;
		item->SetPosition(simon->x + 100, simon->y - 100);
		listItems.push_back(item);
	}
#pragma endregion


}

void Manager::fnChangeScene()
{
	float x, y;
	simon->GetPosition(x, y);

	if (idScene == GAMESTATE1 && simon->changeScene == GAMESTATE2)
		Init(GAMESTATE2);
	else if (idScene == GAMESTATE2 && simon->changeScene == GAMESTATE3)
	{
		Init(GAMESTATE3);
		if (x < MAP_CHECK_2)
			SetGameState(SCENE3_1);
		else
			SetGameState(SCENE3_2);
	}
	else if (idScene == GAMESTATE3 && simon->changeScene == GAMESTATE2)
	{
		Init(SCENE2);
		if (x < MAP_CHECK_3)
			SetGameState(SCENE2_2);
		else
			SetGameState(SCENE2_3);
	}
}

void Manager::Simon_Stair_Down()
{
	int stairDirection = simon->stairDirection;

	if (simon->canMoveDownStair == false)
	{
		if (simon->isStandOnStair == true)
			simon->SetState(SIMON_STAND_IDLE);
		else
			simon->SetState(SIMON_SIT);

		return;
	}

	// Auto-walk của Simon đi đến đúng đầu cầu thang rồi mới bước xuống
	if (simon->isStandOnStair == false)
	{
		float simon_goto_x = simon->goToStair.x;
		float simon_x = simon->x;

		if (stairDirection == -1) simon_goto_x -= 28.0f;

		if (simon_goto_x < simon_x) simon->setNx(-1);
		else if (simon_goto_x > simon_x) simon->setNx(1);
		else return;

		simon->SetState(SIMON_WALK);
		simon->vy = 0;

		simon->AutoWalk(simon_goto_x - simon_x, SIMON_STAIR_DOWN, -stairDirection);
		simon->isStandOnStair = true;

		return;
	}
	else
	{
		simon->setNx(-simon->stairDirection);
		simon->SetState(SIMON_STAIR_DOWN);
	}

	return;
}

void Manager::Simon_Stair_Up()
{
	int stairDirection = simon->stairDirection;

	if (simon->canMoveUpStair == false)
	{
		if (simon->isStandOnStair == true)
		{
			int nx = simon->stairDirection;
			simon->setNx(nx);
			simon->SetState(SIMON_STAIR_UP);
			simon->AutoWalk(15 * nx, SIMON_STAND_IDLE, nx);
		}

		return;
	}

	// Auto-walk của Simon đi đến đúng chân cầu thang rồi mới bước lên
	if (simon->isStandOnStair == false)
	{

		float simon_goto_x = simon->goToStair.x;
		float simon_x = simon->x;

		if (stairDirection == 1)
			simon_goto_x -= 35.0f;
		else 
			simon_goto_x += 9.0f;

		if (simon_goto_x < simon_x) simon->setNx(-1);
		else if (simon_goto_x > simon_x)  simon->setNx(1);
		else return;

		simon->SetState(SIMON_WALK);
		simon->vy = 0;
		simon->AutoWalk(simon_goto_x - simon_x, SIMON_STAIR_UP, stairDirection);
		simon->isStandOnStair = true;

		return;
	}
	else
	{
		simon->setNx(stairDirection);
		simon->SetState(SIMON_STAIR_UP);
	}

	return;
}

bool Manager::Simon_Stair_Stand()
{
	if (simon->GetState() == SIMON_STAIR_UP || simon->GetState() == SIMON_STAIR_DOWN ||
		simon->GetState() == SIMON_HIT_STAIR_UP || simon->GetState() == SIMON_HIT_STAIR_DOWN ||
		simon->GetState() == SIMON_STAND_STAIR_UP || simon->GetState() == SIMON_STAND_STAIR_DOWN)
	{
		if (simon->GetState() == SIMON_HIT_STAIR_UP)
		{
			simon->SetState(SIMON_STAND_STAIR_UP);
		}
		else if (simon->GetState() == SIMON_HIT_STAIR_DOWN)
		{
			simon->SetState(SIMON_STAND_STAIR_DOWN);
		}

		simon->StandOnStair();
		

		return true;
	}

	return false;
}

bool Manager::SimonWalkThroughDoor()
{
	if (simon->isWalkThroughDoor == true && simon->isCoGround == true)
	{
		simon->isWalkThroughDoor = false;
		
		simon->setNx(1);
		simon->SetState(SIMON_STAND_IDLE);

		isMovingCamera1 = true;
		countDxCamera = 0;
	}

	if (isMovingCamera1 == true)
	{
		if (countDxCamera < 240)			// Di chuyển camera một đoạn 240
		{
			countDxCamera += 2;

			D3DXVECTOR2 cam = game->getCamPosition();
			game->setCamPosition(cam.x + 2, cam.y);

			return true;
		}

		if (isSetSimonAutoWalk == false)	// AutoWalk
		{
			isSetSimonAutoWalk = true;

			simon->SetState(SIMON_WALK);
			simon->vy = 0;
			simon->vx = SIMON_WALKING_LOWER_SPEED;
			simon->AutoWalk(120, SIMON_STAND_IDLE, 1);
		}
		else
		{
			if (simon->autoWalk == false)
			{
				isMovingCamera2 = true;

				if (countDxCamera < 620)	// Di chuyển camera thêm một đoạn -> 620
				{
					countDxCamera += 2;

					D3DXVECTOR2 cam = game->getCamPosition();
					game->setCamPosition(cam.x + 2, cam.y);
					return true;
				}
				else
				{
					isMovingCamera1 = false;
					isMovingCamera2 = false;
					isSetSimonAutoWalk = false;
					if(simon->x < MAP_CHECK_2)
						tileMap->LoadMap(SCENE2_1);
					else
						tileMap->LoadMap(SCENE2_BOSS);
					countDxCamera = 0;
				}
			}
		}
	}
	return false;
}

bool Manager::CheckSimonCollisionStair()
{
	return simon->CheckCollisionWithStair(&listStairs);
}

void Manager::ResetGame()
{
	if (simon->isCheckReset == true)
	{
		float x;
		simon->getX(x);
		int life = simon->GetLife();
		if (life <= 0)
			return;
		simon = new Simon();
		if (life > 0)
			simon->SetLife(life);

		ui->SetSimon(simon);
		boss = new Boss();
		boss->SetState(BOSS_SLEEP);

		switch (idScene)
		{
		case GAMESTATE1:
			Init(GAMESTATE1);
			break;
		case GAMESTATE2:

			Init(GAMESTATE2);
			if(x > 3072 && x < 4096)
			SetGameState(SCENE2_1);
			else if( x > 4096)
				SetGameState(SCENE2_BOSS);
			isBossFighting = false;
			break;
		case GAMESTATE3:
			Init(GAMESTATE2);
			SetGameState(SCENE2_1);
			break;

		default:
			break;
		}
	}
}

void Manager::CrossEffect()
{
	if (simon->isCoCross == true)
	{
		for (UINT i = 0; i < listGridObjects.size(); i++)
		{
			// Cross chỉ tác dụng với các object nằm trong viewport
			if (IsInViewport(listGridObjects[i]) == false)
				continue;
			if (listGridObjects[i]->isDestroy == true)
				continue;
			if (dynamic_cast<Zombie*>(listGridObjects[i]) && listGridObjects[i]->GetState() == ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(listGridObjects[i]);
				zombie->isDestroy = true;
			}
			else if (dynamic_cast<Panther*>(listGridObjects[i]) && listGridObjects[i]->GetState() == ACTIVE)
			{
				auto panther = dynamic_cast<Panther*>(listGridObjects[i]);
				panther->isDestroy = true;
			}
			else if (dynamic_cast<Bat*>(listGridObjects[i]) && listGridObjects[i]->GetState() == ACTIVE)
			{
				auto bat = dynamic_cast<Bat*>(listGridObjects[i]);
				bat->isDestroy = true;
			}
			else if (dynamic_cast<FishMan*>(listGridObjects[i]) && listGridObjects[i]->GetState() == ACTIVE)
			{
				auto fishman = dynamic_cast<FishMan*>(listGridObjects[i]);
				fishman->isDestroy = true;
			}
		}
	}
}
