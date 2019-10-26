#include "Stage1.h"


Stage1::Stage1()
{
	canControl = true;
	LoadResources();
}


Stage1::~Stage1()
{
}


void Stage1::LoadResources()
{
	timeDelay = 0;
	//changeState = false;


	tileMap = new TileMap(STAGE1,32,32);
	simon = new Simon();
	simon->SetPosition(20, 300);
	objects.push_back(simon);
	/*ui = new UI();
	ui->setDevice(game->GetDirect3DDevice());
	ui->setSimon(simon);
	ui->LoadResources();
	time = 300;*/





	for (int i = 0; i < 48; i++)
	{
		Ground *ground = new Ground();
		ground->SetState(ANI_GROUND_LV1);
		ground->SetPosition(0 + i * 32, 480 - 62);
		objects.push_back(ground);
	}


	for (int i = 0; i < 6; i++)
	{
		Candle *candle = new Candle();
		candle->isDropItem = true;
		candle->SetState(ANI_BIG_CANDLE);
		candle->SetPosition(160 + i * 270, 480 - 128);
		objects.push_back(candle);
	}
	//Countdown = 0;
	weapon = new SubWeapon();
	weapon->SetDestroy(true);
	weaponlist.push_back(weapon);
}



void Stage1::Update(DWORD gameTime)
{

	//time = 300 - Countdown / 100.;
	//ui->Update(64, time, 3, 1);
	//Countdown++;
	if (canControl)
		Control();


	vector<LPGAMEOBJECT> listObject;
	for (int i = 1; i < objects.size() + listItem.size(); i++)
	{
		if (i <= objects.size() - 1)
			listObject.push_back(objects[i]);
		else
			listObject.push_back(listItem[i - objects.size()]);
	}


	//for (int i = 1; i < objects.size(); i++)
	//{
	//	listObject.push_back(objects[i]);
	//}


	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isDestroy == false)
			objects[i]->Update(gameTime, &listObject);
		else
		{
			deleteObject(objects[i], i);
		}
	}

	for (int i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->isDestroy != true)
			listItem[i]->Update(gameTime, &listObject);
		else
			listItem.erase(listItem.begin() + i);
	}


	if (weaponlist[0]->isDestroy != true)
	{
		weaponlist[0]->Update(gameTime, &listObject);
	}


	if (simon->x >= SCREEN_WIDTH / 2 && simon->x <= MAP_WIDTH_LV1 - SCREEN_WIDTH / 2 - 32)
	{
		simon->getX(pos.x);
		pos.x -= SCREEN_WIDTH / 2;
		game->setCamPosition(pos.x, 0);
	}

	//if (simon->x > 1280 && simon->x < 1420)
	//{
	//	simon->SetState(SIMON_WALK);
	//	canControl = false;
	//	if (simon->x > 1350)
	//		SetChangingState(true);

	//}

}

void Stage1::Render()
{
	tileMap->DrawMap(pos);
	//ui->Render();
	for (int i = 1; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	simon->Render();
	for (int i = 0; i < listItem.size(); i++)
	{
		listItem[i]->Render();
	}
	weaponlist[0]->Render();
}

bool Stage1::GetChangingState()
{
	return GameStage::GetChangingStage();
}

void Stage1::SetChangingState(bool status)
{
	GameStage::SetChangingStage(status);
}

void Stage1::DestroyAll()
{
	SAFE_DELETE(tileMap);
	objects.clear();

}

void Stage1::Control()
{
	
	/*if ((simon->GetState() == SIMON_JUMP || simon->GetState() == SIMON_STAND_IDLE) && simon->isCoGround == false)
		return;*/
	if (simon->isAttack() || simon->GetState() == SIMON_EFFECT)
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
			if (weaponlist[0]->isDestroy == false)
			{
				return;
			}
			float x, y;
			simon->GetPosition(x, y);

			if (simon->GetState() == SIMON_SIT) y += 25.0f; // khớp vị trí tay
			else y += 10.0f;
			if (simon->nx < 0) x += 30.0f;

			weaponlist[0]->SetPosition(x, y);

			// nx
			weaponlist[0]->setNx(simon->nx);

			// state enable
			weaponlist[0]->SetDestroy(false);
			weaponlist[0]->SetState(simon->SubWeapon);
			simon->isHitSubWeapon = true;
			if(simon->GetState() == SIMON_SIT)
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


void Stage1::deleteObject(LPGAMEOBJECT object, int i)
{

	if (object->isDropItem == true)
	{
		Items *item = new Items();
		if (simon->isMaxLevelWhip())
			item->isMax();
		item->Random();
		item->isEffect = false;
		item->SetPosition(object->x, object->y);
		listItem.push_back(item);
	}

	if (object->isEffect == false)
	{
		EffectDead *effect = new EffectDead();
		effect->SetState(EFFECTDEAD_STAR);
		effect->SetPosition(object->x, object->y);
		effect->isEffect = true;
		objects.push_back(effect);

		effect = new EffectDead();
		effect->SetState(EFFECTDEAD_FIRE);
		effect->isEffect = true;
		effect->SetPosition(object->x, object->y);
		objects.push_back(effect);
	}
	
	objects.erase(objects.begin() + i);
}