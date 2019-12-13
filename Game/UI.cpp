#include "UI.h"

UI::UI()
{

	AddAnimation("simon_hp_ani");
	AddAnimation("no_hp_ani");
	AddAnimation("boss_hp_ani");
	AddAnimation("item_knife_ani");
	AddAnimation("item_boomerang_ani");
	AddAnimation("item_axe_ani");
	AddAnimation("item_stop_watch_ani");
	AddAnimation("item_holywater_ani");
	AddAnimation("blackbroad_ani");


	font = NULL;
	SetRect(&rect, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
	information = "SCORE-000000 TIME 0000 SCENE 00\n";
	information += "PLAYER                  -00\n";
	information += "ENEMY                   -00\n";
}

UI::~UI()
{
}


void UI::Update(int bossHP,DWORD dt, int stage)
{

	this->bossHP = bossHP;



	time += dt;
	int remainTime = PLAY_TIME - (time / CLOCKS_PER_SEC);


	string scoreString = to_string(score);
	while (scoreString.length() < 6) scoreString = "0" + scoreString;

	string timeString = to_string(remainTime);
	while (timeString.length() < 4) timeString = "0" + timeString;

	string sceneString = to_string(stage);
	while (sceneString.length() < 2) sceneString = "0" + sceneString;

	string heartString = to_string(simon->GetHeart());
	while (heartString.length() < 2) heartString = "0" + heartString;

	string lifeString = to_string(simon->GetLife());
	while (lifeString.length() < 2) lifeString = "0" + lifeString;


	information = "SCORE_" + scoreString + " TIME " + timeString + " STAGE " + sceneString + "\n";
	information += "PLAYER                 " + heartString + "\n";
	information += "ENEMY                  " + lifeString + "\n";
}

void UI::Render()
{
	animations[BLACKBROAD]->DrawBoard(330, 42);

	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	for (int i = 0; i < 16; i++)
		if (i < simon->GetHP())
			animations[SIMON_HP]->DrawBoard(130 + 12 * i, 42);//42
		else
			animations[NO_HP]->DrawBoard(130 + 12 * i, 42);

	for (int i = 0; i < 16; i++)
		if (i < bossHP)
			animations[BOSS_HP]->DrawBoard(130 + 12 * i, 68); //68
		else
			animations[NO_HP]->DrawBoard(130 + 12 * i, 68);
	
	switch (simon->GetSubWeapon())
	{
	case ITEM_KNIFE:
		animations[ITEM_KNIFE]->DrawBoard(348, 60);
		break;
	case ITEM_BOOMERANG:
		animations[ITEM_BOOMERANG]->DrawBoard(348, 60);
		break;
	case ITEM_AXE:
		animations[ITEM_AXE]->DrawBoard(348, 60);
		break;
	case ITEM_STOP_WATCH:
		animations[ITEM_STOP_WATCH]->DrawBoard(348, 60);
		break;
	case ITEM_HOLY_WATER:
		animations[ITEM_HOLY_WATER]->DrawBoard(348, 60);
		break;
	default:
		break;
	}


	/*if (simon == ITEM_ANI_DOUBLE)
		animations[9]->DrawBoard(520, 60);
	else if (simon == ITEM_ANI_TRIPLE)
	{
		animations[10]->DrawBoard(520, 60);
	}*/



}

void UI::AddAnimation(string aniId)
{
	LPANIMATION ani = Animations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}
