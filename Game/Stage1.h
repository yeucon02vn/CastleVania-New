#pragma once
#include "GameStage.h"
#include "Ground.h"
#include "Candle.h"
#include "Items.h"
//#include "UI.h"
#include "EffectDead.h"
#include "SubWeapon.h"
class Stage1 : public GameStage
{
private:
	TileMap *tileMap;
	float timeDelay;
	D3DXVECTOR2 pos;
	vector<LPGAMEOBJECT> objects;
	vector<LPITEMS> listItem;
	vector<SubWeapon*> weaponlist;
	bool canControl;
public:
	Stage1();
	~Stage1();
	void Render();
	void Update(DWORD gameTime);
	void DestroyAll();

	void LoadResources();

	bool GetChangingState();
	void SetChangingState(bool status);
	void Control();
	void deleteObject(LPGAMEOBJECT objects, int i);

};

