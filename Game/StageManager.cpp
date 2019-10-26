
#include "StageManager.h"

///*CHUYEN CAC THANH PHAN CO BAN TU GAME.CPP SANG*/

StageManager::StageManager()
{
}


StageManager::~StageManager()
{
}


void StageManager::LoadStage(int stageID)
{
	switch (stageID)
	{
	case STAGE1:
		delete(gameStage);
		gameStage = new Stage1();
		gameStage->stage = stageID;
		this->stageID = stageID;
		break;
	/*case STAGE2:
		delete(gameStage);
		gameStage = new Stage2();

		gameStage->stage = StageID;
		this->StageID = StageID;
		break;*/
	default:
		break;
	}
}

void StageManager::NextStageLevel()
{
	stageID++;
}
void StageManager::Render()
{
	gameStage->Render();
}

void StageManager::Update(float gameTime)
{
	//if (gameStage->GetChangingStage())
	//{
	//	//if (StageID != MENU_Stage)
	//	//{
	//		gameStage->DestroyAll();
	//	//}
	//	NextStageLevel();
	//	LoadStage(StageID);
	//	gameStage->SetChangingStage(false);
	//}
	gameStage->Update(gameTime);
}

