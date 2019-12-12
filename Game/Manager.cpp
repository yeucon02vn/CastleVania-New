#include "Manager.h"

Manager::Manager(Game * game)
{
	this->game = game;

	// INIT
	simon = new Simon();
	canControl = true;
	for (int i = 0; i < 3; i++)
	{
		weapon = new SubWeapon();
		weapon->SetDestroy(true);
		listWeapon.push_back(weapon);
	}
	tileMap = new TileMap(FRAME_TILE_MAP, FRAME_TILE_MAP);
	tileMap->LoadResource();
	Init(GAMESTATE3);
}

Manager::~Manager()
{
}

void Manager::Init(int idScene)
{
	this->idScene = idScene;
	switch (idScene)
	{
	case GAMESTATE1:
		grid = new Grid(1536, 480);
		LoadObjectsFromFile(L"Scenes\\Scene1_objects.txt");
		SetGameState(GAMESTATE1);
		break;
	case GAMESTATE2:
		grid = new Grid(5632, 480);
		LoadObjectsFromFile(L"Scenes\\Scene2_objects.txt");
		SetGameState(SCENE2);
		break;
	case GAMESTATE3:
		grid = new Grid(1024, 480);
		LoadObjectsFromFile(L"Scenes\\Scene3_objects.txt");
		//simon->SetState(SIMON_STAIR_DOWN);
		SetGameState(SCENE3_1);
		break;
	default:
		break;
	}
}

void Manager::LoadObjectsFromFile(LPCWSTR FilePath)
{
	fstream fs;

	fs.open(FilePath, ios::in);
	if (fs.fail())
	{		
		fs.close();
		return;
	}

	int ID_Obj;
	float pos_x, pos_y;
	int state;
	bool isEnable;

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isEnable;

		switch (ID_Obj)
		{
		case CANDLE:
		{
			Candle * candle = new Candle();
			candle->SetPosition(pos_x, pos_y);
			candle->SetState(state);
			candle->isDropItem = true;
			unit = new Unit(grid, candle, pos_x, pos_y);
			break;
		}
		case GROUND:
		{
			Ground * ground = new Ground();
			ground->SetPosition(pos_x, pos_y);
			ground->SetState(state);
			
			unit = new Unit(grid, ground, pos_x, pos_y);
			break;
		}

		case STAIR:
		{
			Stair * stair = new Stair();
			stair->SetPosition(pos_x, pos_y);
			stair->SetState(state);
			unit = new Unit(grid, stair, pos_x, pos_y);
			break;
		}
		case DOOR:
		{
			Door * door = new Door();
			door->SetPosition(pos_x, pos_y);
			door->SetState(state);
			unit = new Unit(grid, door, pos_x, pos_y);
			break;
		}
		case ZOMBIE:
		{
			Zombie * zombie = new Zombie();
			zombie->SetEntryPosition(pos_x, pos_y);
			zombie->SetState(ZOMBIE_IDLE);
			unit = new Unit(grid, zombie, pos_x, pos_y);
			break;
		}
		case PANTHER:
		{
			Panther * panther = new Panther();
			panther->SetEntryPosition(pos_x, pos_y);
			panther->SetState(PANTHER_IDLE_INACTIVE);
			unit = new Unit(grid, panther, pos_x, pos_y);
			break;
		}
		case BAT:
		{
			Bat * bat = new Bat();
			bat->SetEntryPosition(pos_x, pos_y);
			bat->SetState(BAT_IDLE);
			unit = new Unit(grid, bat, pos_x, pos_y);
			break;
		}
		case FISHMAN:
		{
			FishMan * fishman = new FishMan();
			fishman->SetEntryPosition(pos_x, pos_y);
			fishman->SetState(FISHMAN_IDLE);
			unit = new Unit(grid, fishman, pos_x, pos_y);
			break;
		}
		case CHANGE_SCENE_OBJECT:
		{
			ChangeScene * changeScene = new ChangeScene();
			changeScene->SetPosition(pos_x, pos_y);
			changeScene->SetIdScene(state);
			unit = new Unit(grid, changeScene, pos_x, pos_y);
			break;
		}
		case WATER:
		{
			water = new Water();
			water->SetPosition(pos_x, pos_y);
			unit = new Unit(grid, water, pos_x, pos_y);
			break;
		}
		case BREAKWALL:
		{
			BreakWall * breakwall = new BreakWall();
			breakwall->SetPosition(pos_x, pos_y);
			breakwall->SetState(HIDDEN);
			breakwall->isDropItem = true;
			unit = new Unit(grid, breakwall, pos_x, pos_y);
			break;
		}

		default:
			break;
		}
	}

	fs.close();
}

void Manager::GetObjectFromGrid()
{
	listUnits.clear();
	listGridObjects.clear();
	listStairs.clear();
	listDoors.clear();
	listStaticObjects.clear();
	listMovingObjects.clear();

	grid->Get(game->getCamPosition(), listUnits);

	//DebugOut(L"%d \n", listUnits.size());

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
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

		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Candle*>(obj) || dynamic_cast<FireBall*>(obj))
				coObjects.push_back(obj);
			else if ((dynamic_cast<Zombie*>(obj) || dynamic_cast<Bat*>(obj))
				&& obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
			else if (dynamic_cast<Panther*>(obj) &&
				(obj->GetState() == PANTHER_ACTIVE || obj->GetState() == PANTHER_IDLE || obj->GetState() == PANTHER_JUMP))
				coObjects.push_back(obj);
			else if (dynamic_cast<FishMan*>(obj) &&
				(obj->GetState() == ACTIVE || obj->GetState() == FISHMAN_JUMP || obj->GetState() == FISHMAN_HIT))
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

	
	vector<LPGAMEOBJECT> listCoObjects;
	GetObjectFromGrid();

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
		}
		else
		{
			DeleteObject(listGridObjects[i], i);
		}
	}

	for (int i = 0; i < listEffects.size(); i++)
	{
		listEffects[i]->Update(dt);
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


	if (listWeapon[0]->isDestroy != true)
	{
		LPGAMEOBJECT object = listWeapon[0];
		listCoObjects.clear();
		GetColliableObjects(object, listCoObjects);
		listWeapon[0]->Update(dt, &listCoObjects);
	}
	// update vị trí của quái vào cell mới
	UpdateGrid();

	
	
	UpdateCam();

	SetInactivationByPosition();

	fnChangeScene();
}

void Manager::UpdateCam()
{
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
	//ui->Render();
	
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
	//listWeapon[0]->Render();
}

void Manager::UpdateGrid()
{
	GetObjectFromGrid();
	for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		if (dynamic_cast<Enemy*>(obj))
		{
			float newPos_x, newPos_y;
			obj->GetPosition(newPos_x, newPos_y);
			listUnits[i]->Move(newPos_x, newPos_y);
		}
	}
}

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
		if (dynamic_cast<BreakWall*>(object))
		{
			object->isDestroy = false;
			return;
		}
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
		return;
	}
	else
	{
		grid->Delete(listUnits[i]);
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

	for (int i = 0; i < 3; i++)
	{
		if (listWeapon[i]->isDestroy == false)
		{
			if (IsInViewport(listWeapon[i]) == false)
				listWeapon[i]->isDestroy = true;
		}
	}
}

void Manager::SetEnemiesSpawnPositon()
{
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
	if (simon->isAttack() || simon->GetState() == SIMON_EFFECT || simon->autoWalk || simon->GetState() == SIMON_DEFLECT || simon->GetState() == SIMON_DIE || simon->isFinishDelayAnimation == false)
		return;

	if (IsKeyPress(DIK_SPACE))
	{
		if (simon->isCoGround == false || simon->GetState() == SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
	}
	else if (IsKeyDown(DIK_K))
	{
		simon->SetPosition(simon->x + 50, simon->y);
	}
	else if (IsKeyDown(DIK_UP))
	{
		if (CheckSimonCollisionStair() == true)
		{
			Simon_Stair_Up();
			return;
		}

		if (IsKeyPress(DIK_Z))
		{
			//listWeapon[0]->isDestroy == true;

			SubWeapon *weapon;
			if (simon->GetSubWeapon() == -1)
			{
				return;
			}

			if (listWeapon[0]->isDestroy == true)
				weapon = listWeapon[0];
			else return;

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
				if(simon->stairDirection == 1)
					simon->SetState(SIMON_HIT_STAIR_UP);
				else
					simon->SetState(SIMON_HIT_STAIR_DOWN);
			else
				if (simon->stairDirection == 1)
					simon->SetState(SIMON_HIT_STAIR_DOWN);
				else
					simon->SetState(SIMON_HIT_STAIR_UP);
		}
		if (simon->GetState() == SIMON_SIT)
			simon->SetState(SIMON_SIT_ATTACK);
		else if (simon->GetState() == SIMON_STAND_IDLE || simon->GetState() == SIMON_JUMP)
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
		if (simon->isCoGround == false)
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
		if (simon->isCoGround == false)
			return;
		simon->SetState(SIMON_WALK);
		simon->setNx(1);
	}

	else if (IsKeyDown(DIK_DOWN))
	{
		if (CheckSimonCollisionStair() == true)
		{
			Simon_Stair_Down();
			return;
		}
		if (simon->isCoGround == false)
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
	if (IsKeyDown(DIK_1))
	{
		Init(SCENE1);
	}
	else if (IsKeyDown(DIK_2))
	{
		Init(SCENE2);
	}
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
		if (x < 3500)
			SetGameState(SCENE3_1);
		else
			SetGameState(SCENE3_2);
	}
	else if (idScene == GAMESTATE3 && simon->changeScene == GAMESTATE2)
	{
		Init(SCENE2);
		if (x < 300)
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
			simon->AutoWalk(16 * nx, SIMON_STAND_IDLE, nx);
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
		if (countDxCamera < 240)			// Di chuyển camera một đoạn 224
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

				if (countDxCamera < 640)	// Di chuyển camera thêm một đoạn -> 480
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
					tileMap->LoadMap(SCENE2_1);
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
