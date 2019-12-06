#include "Manager.h"

Manager::Manager(Game * game)
{
	this->game = game;

	// INIT
	simon = new Simon();
	canControl = true;
	for (int i = 1; i <= 3; i++)
	{
		weapon = new SubWeapon();
		weapon->SetDestroy(true);
		listWeapon.push_back(weapon);
	}
	tileMap = new TileMap();
	Init(SCENE1);
}

Manager::~Manager()
{
}

void Manager::Init(int idScene)
{
	this->idScene = idScene;
	switch (idScene)
	{
	case SCENE1:
		grid = new Grid(1536, 480);
		LoadObjectsFromFile(L"Scenes\\Scene1_objects.txt");
		SetGameState(SCENE1);
		break;
	case SCENE2:
		grid = new Grid(5632, 480);
		LoadObjectsFromFile(L"Scenes\\Scene2_objects.txt");
		SetGameState(SCENE2);
		break;
	case SCENE3:
		grid = new Grid(1024, 480);
		LoadObjectsFromFile(L"Scenes\\Scene3_objects.txt");
		//simon->SetState(STAIR_DOWN);
		SetGameState(SCENE3);
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
			zombie->isDropItem = true;
			unit = new Unit(grid, zombie, pos_x, pos_y);
			break;
		}
		case PANTHER:
		{
			Panther * panther = new Panther();
			panther->SetEntryPosition(pos_x, pos_y);
			panther->SetState(PANTHER_IDLE_INACTIVE);
			panther->isDropItem = true;
			unit = new Unit(grid, panther, pos_x, pos_y);
			break;
		}
		case BAT:
		{
			Bat * bat = new Bat();
			bat->SetEntryPosition(pos_x, pos_y);
			bat->SetState(BAT_IDLE);
			bat->isDropItem = true;
			unit = new Unit(grid, bat, pos_x, pos_y);
			break;
		}
		case FISHMAN:
		{
			FishMan * fishman = new FishMan();
			fishman->SetEntryPosition(pos_x, pos_y);
			fishman->SetState(FISHMAN_IDLE);
			fishman->isDropItem = true;
			unit = new Unit(grid, fishman, pos_x, pos_y);
			break;
		}
		case WATER:
		{
			Water * water = new Water();
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
		case CHANGE_SCENE_OBJECT:
		{
			ChangeScene * changeScene = new ChangeScene();
			changeScene->SetPosition(pos_x, pos_y);
			changeScene->SetIdScene(state);
			unit = new Unit(grid, changeScene, pos_x, pos_y);
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



	grid->Get(game->getCamPosition(), listUnits);

	//DebugOut(L"%d \n", listUnits.size());

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		listGridObjects.push_back(obj);
	}
}


void Manager::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Items*>(curObj))
	{
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Ground*>(obj))
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

	/*	if (isBossFighting == true && boss->GetState() == BOSS_ACTIVE && weapon->GetTargetTypeHit() != BOSS)
			coObjects.push_back(boss);
*/
		for (auto obj : listGridObjects)
		{
			if (dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj))
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
				dynamic_cast<ChangeScene*>(obj) || dynamic_cast<Candle*>(obj))
				coObjects.push_back(obj);
			else if (dynamic_cast<BreakWall*>(obj) && obj->GetState() == HIDDEN)
				coObjects.push_back(obj);
			else if (dynamic_cast<FireBall*>(obj) && obj->isDestroy == false)
				coObjects.push_back(obj);
			else if ((dynamic_cast<Zombie*>(obj) || dynamic_cast<Panther*>(obj) ||
				dynamic_cast<Bat*>(obj)) && obj->GetState() == ACTIVE)
				// || dynamic_cast<Boss*>(obj))
				coObjects.push_back(obj);
			else if (dynamic_cast<FishMan*>(obj) && (obj->GetState() == ACTIVE || obj->GetState() == FISHMAN_JUMP))
				coObjects.push_back(obj);
		}
		for(auto obj : listItems)
			coObjects.push_back(obj);
	}
}


void Manager::Update(DWORD dt)
{
	if (canControl)
		Control();


	vector<LPGAMEOBJECT> listCoObjects;
	GetObjectFromGrid();
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
			listGridObjects[i]->Update(dt, &listCoObjects);
		else
		{
			DeleteObject(listGridObjects[i], i);
		}
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

	/*for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		float newPos_x, newPos_y;
		obj->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
	}*/
	fnChangeScene();
	if (idScene == SCENE1)
	{
		if (simon->x >= SCREEN_WIDTH / 2 && simon->x < 1536 - 320 )
		{
			simon->getX(pos.x);
			pos.x -= SCREEN_WIDTH / 2;
			game->setCamPosition(pos.x, 0);
		}
	}
	else
	{
		if (simon->x >= SCREEN_WIDTH / 2)
		{
			simon->getX(pos.x);
			pos.x -= SCREEN_WIDTH / 2;
			game->setCamPosition(pos.x, 0);
		}
	}
}

void Manager::Render()
{
	tileMap->DrawMap(game->getCamPosition());
	//ui->Render();
	for (int i = 0; i < listGridObjects.size(); i++)
	{
		listGridObjects[i]->Render();
	}
	simon->Render();
	for (int i = 0; i < listItems.size(); i++)
	{
		listItems[i]->Render();
	}

	listWeapon[0]->Render();
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
	}

	if (object->isEffect == false)
	{
		EffectDead *effect = new EffectDead();
		effect->SetState(EFFECTDEAD_STAR);
		effect->SetPosition(object->x, object->y);
		effect->isEffect = true;
		listGridObjects.push_back(effect);

		effect = new EffectDead();
		effect->SetState(EFFECTDEAD_FIRE);
		effect->isEffect = true;
		effect->SetPosition(object->x, object->y);
		listGridObjects.push_back(effect);
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
		tileMap->InitMap(SCENE1, MAP_WIDTH_LV1, MAP_HEIGHT_LV1, FRAME_TILE_MAP, FRAME_TILE_MAP);
		break;
	case SCENE2:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(0, 335);
		game->setCamPosition(0, 0);
		simon->autoWalk = false;
		tileMap->InitMap(SCENE2, MAP_WIDTH_LV2, MAP_HEIGHT_LV2, FRAME_TILE_MAP, FRAME_TILE_MAP);
		break;
	case SCENE3:
		simon->SetState(SIMON_STAND_IDLE);
		simon->SetPosition(3115, 143);
		game->setCamPosition(3056, 0);
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
	if (simon->isAttack() || simon->GetState() == SIMON_EFFECT || simon->autoWalk)
		return;

	if (IsKeyPress(DIK_SPACE))
	{
		if (simon->isCoGround == false || simon->GetState() == SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
	}
	else if (IsKeyDown(DIK_UP))
	{
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
		if (simon->GetState() == SIMON_SIT)
			simon->SetState(SIMON_SIT_ATTACK);
		else if (simon->GetState() == SIMON_STAND_IDLE || simon->GetState() == SIMON_JUMP)
			simon->SetState(SIMON_STAND_ATTACK);

	}
	else if (IsKeyDown(DIK_LEFT))
	{
		if (simon->isCoGround == false)
			return;
		simon->SetState(SIMON_WALK);
		simon->setNx(-1);
	}
	else if (IsKeyDown(DIK_RIGHT))
	{
		if (simon->isCoGround == false)
			return;
		simon->SetState(SIMON_WALK);
		simon->setNx(1);
	}

	else if (IsKeyDown(DIK_DOWN))
	{
		if (simon->isCoGround == false)
			return;
		simon->SetState(SIMON_SIT);
	}
	else
	{
		if (simon->isCoGround == false || simon->isAttack() == true)
			return;
		simon->SetState(SIMON_STAND_IDLE);
	}
}

void Manager::fnChangeScene()
{
	 if (idScene == SCENE1 && simon->changeScene == SCENE2)
		Init(SCENE2);
}