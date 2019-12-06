//#include "UI.h"
//
//UI::UI()
//{
//	AddAnimation("");
//	AddAnimation("");
//	AddAnimation("");
//	AddAnimation("");
//}
//
//UI::~UI()
//{
//}
//
//
//void UI::Update(int bossHP, int time, int life, int stage)
//{
//	this->bossHP = bossHP;
//	this->time = time;
//	this->stage = stage;
//
//
//	std::string scoreString = std::to_string(simon->getScore());
//	while (scoreString.length() < 6)
//		scoreString = "0" + scoreString;
//
//
//	std::string timeString = std::to_string(this->time);
//	while (timeString.length() < 4)
//		timeString = "0" + timeString;
//
//	std::string stageString = std::to_string(this->stage);
//	while (stageString.length() < 2)
//		stageString = "0" + stageString;
//
//
//	information = "SCORE_" + scoreString + " TIME " + timeString + " STAGE " + stageString + "\n";
//	information += "PLAYER                 ";// +std::to_string(simon->getHeart()) + "\n";
//	information += "ENEMY                  ";// +std::to_string(simon->getLife()) + "\n";
//}
//
//void UI::Render()
//{
//	animations[0]->DrawBoard(330, 42);
//
//	if (font)
//		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
//
//	for (int i = 0; i < 16; i++)
//		if (i < simon->GetHP())
//			animations[2]->DrawBoard(130 + 12 * i, 42);
//		else
//			animations[3]->DrawBoard(130 + 12 * i, 42);
//
//	for (int i = 0; i < 16; i++)
//		if (i < bossHP)
//			animations[1]->DrawBoard(130 + 12 * i, 68);
//		else
//			animations[3]->DrawBoard(130 + 12 * i, 68);
//	
//	switch (simon->GetSubWeapon())
//	{
//	case ITEM_KNIFE:
//		animations[4]->DrawBoard(348, 60);
//		break;
//	case ITEM_AXE:
//		animations[5]->DrawBoard(348, 60);
//		break;
//	case ITEM_BOOMERANG:
//		animations[6]->DrawBoard(348, 60);
//		break;
//	case ITEM_HOLYWATER:
//		animations[7]->DrawBoard(348, 60);
//		break;
//	case ITEM_STOP_WATCH:
//		animations[8]->DrawBoard(348, 60);
//		break;
//	default:
//		break;
//	}
//
//
//	/*if (simon->updateDame == ITEM_ANI_DOUBLE)
//		animations[9]->DrawBoard(520, 60);
//	else if (simon->updateDame == ITEM_ANI_TRIPLE)
//	{
//		animations[10]->DrawBoard(520, 60);
//	}*/
//
//
//
//}
