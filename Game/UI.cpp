//#include "UI.h"
//
//UI::UI()
//{
//}
//
//UI::~UI()
//{
//}
//
//void UI::LoadResources()
//{
//	
//}
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
//	information += "PLAYER                 " + std::to_string(simon->getHeart()) + "\n";
//	information += "ENEMY                  " + std::to_string(simon->getLife()) + "\n";
//}
//
//void UI::Render()
//{
//	sprite[0]->DrawBoard(330, 42);
//
//	if (font)
//		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
//
//	for (int i = 0; i < 16; i++)
//		if (i < simon->GetHP())
//			sprite[2]->DrawBoard(130 + 12 * i, 42);
//		else
//			sprite[3]->DrawBoard(130 + 12 * i, 42);
//
//	for (int i = 0; i < 16; i++)
//		if (i < bossHP)
//			sprite[1]->DrawBoard(130 + 12 * i, 68);
//		else
//			sprite[3]->DrawBoard(130 + 12 * i, 68);
//	if (simon->currentItem == ITEM_ANI_KNIFE)
//		sprite[4]->DrawBoard(348, 60);
//	else if (simon->currentItem == ITEM_ANI_AXE)
//		sprite[5]->DrawBoard(348, 60);
//	else if (simon->currentItem == ITEM_ANI_CROSS)
//		sprite[6]->DrawBoard(348, 60);
//	else if (simon->currentItem == ITEM_ANI_HOLYWATER)
//		sprite[7]->DrawBoard(348, 60);
//	else if (simon->currentItem == ITEM_ANI_STOPWATCH)
//		sprite[8]->DrawBoard(348, 60);
//	if (simon->updateDame == ITEM_ANI_DOUBLE)
//		sprite[9]->DrawBoard(520, 60);
//	else if (simon->updateDame == ITEM_ANI_TRIPLE)
//	{
//		sprite[10]->DrawBoard(520, 60);
//	}
//
//
//
//}
