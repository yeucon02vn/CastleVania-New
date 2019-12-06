//#pragma once
//#include "d3dx9.h"
//#include "Define.h"
//#include <string>
//#include <vector>
//#include "Simon.h"
//
//class UI
//{
//private:
//	ID3DXFont *font;
//	RECT rect;
//	Simon *simon;
//
//	std::string information;
//	vector<LPANIMATION> animations;
//
//
//	int simonHP;
//	int bossHP;
//
//	int score;
//	int time;
//	int item;
//	int energy;
//	int life;
//	int stage;
//	int subWeapon;
//
//public:
//	UI();
//	~UI();	void Update(int bossHP, int time, int life, int stage);
//	void Render();
//	void setSimon(Simon *simon)
//	{
//		this->simon = simon;
//		this->bossHP = 16;
//	}
//	void AddAnimation(string aniId)
//	{
//		LPANIMATION ani = Animations::GetInstance()->Get(aniId);
//		animations.push_back(ani);
//	}
//};