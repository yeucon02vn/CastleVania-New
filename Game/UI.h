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
//	LPDIRECT3DDEVICE9 d3ddv;
//	vector<LPANIMATION> sprite;
//	//std::vector<Sprite*> *simonHPList;
//	//std::vector<Sprite*> *enemyHPList;
//	//std::vector<Sprite*> *noHPList;
//
//	//Sprite* heart;
//	//Sprite* weapon;
//	//Sprite* black;
//	//Sprite* axe;
//	//Sprite* holyWater;
//	//Sprite* cross;
//	//Sprite* knife;
//	//Sprite* stopWatch;
//
//	//Sprite* upgrade1;
//	//Sprite* upgrade2;
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
//	~UI();
//	void LoadResources();
//	void Update(int bossHP, int time, int life, int stage);
//	void Render();
//	void setSimon(Simon *simon)
//	{
//		this->simon = simon;
//		this->bossHP = 16;
//	}
//	void setDevice(LPDIRECT3DDEVICE9 device) { this->d3ddv = device; }
//	//setBossHp
//};