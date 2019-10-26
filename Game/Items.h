#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Ground.h"

class Items : public GameObject
{
private:
	DWORD timeDelete;   // dùng để hủy item sau 1 khoảng time
	bool checkMaxLevelWhip;
public:
	Items();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	int Random();
	void RandomItem();
	void RandomSubWeapon();
	void isMax() { checkMaxLevelWhip = true; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

typedef Items * LPITEMS;
