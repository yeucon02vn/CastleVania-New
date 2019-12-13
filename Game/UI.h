#pragma once
#include "d3dx9.h"
#include "Define.h"
#include <string>
#include <vector>
#include "Simon.h"

class UI
{
private:
	ID3DXFont *font;
	RECT rect;
	Simon *simon;

	std::string information;
	vector<LPANIMATION> animations;
	LPDIRECT3DDEVICE9 d3ddv;

	int time;
	int simonHP;
	int bossHP;
	int score;
	int item;
	int subWeapon;

public:
	UI();
	~UI();
	void Update(int bossHP, DWORD time, int stage);
	void Render();
	void SetSimon(Simon *simon) { this->simon = simon; }
	void AddAnimation(string aniId);
	void SetDevice(LPDIRECT3DDEVICE9 device) { this->d3ddv = device; }
	void SetFont(ID3DXFont *font) { this->font = font; }
	void ResetTime() { time = 0; } // reset nếu đổi scene;
};