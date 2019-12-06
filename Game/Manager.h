#pragma once
#include "Game.h"
#include "Ground.h"
#include "Candle.h"
#include "Items.h"
#include "Simon.h"
#include "SubWeapon.h"
#include <fstream>
#include "ChangeScene.h"
#include "EffectDead.h"
#include "Grid.h"
#include "TileMap.h"
#include "Door.h"
#include "Enemy.h"
#include "Zombie.h"
#include "BreakWall.h"
#include "Stair.h"
#include "Bat.h"
#include "Panther.h"
#include "Fishman.h"
#include "Water.h"
#include "Fireball.h"
using namespace std;
class Manager {
	int idScene;


	Grid * grid;
	Unit * unit;
	Game *game;
	vector<Unit*> listUnits;
	vector<LPGAMEOBJECT> listGridObjects;
	vector<LPITEMS> listItems;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listDoors;
	D3DXVECTOR2 pos;
	Simon * simon;
	SubWeapon * weapon;
	vector<SubWeapon*> listWeapon;
	TileMap * tileMap;
	Water * water;
	bool canControl;

public:
	Manager(Game * game);
	~Manager();
	void Init(int idScene);
	void LoadObjectsFromFile(LPCWSTR FilePath);
	void GetObjectFromGrid();
	int GetIdScene() { return this->idScene; }
	Simon * GetSimon() { return this->simon; }
	vector<SubWeapon*> * GetWeapon() { return &listWeapon; }
	void Update(DWORD dt);
	void Render();
	void DeleteObject(LPGAMEOBJECT object, int i);
	void SetGameState(int gameState);
	void Control();
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void fnChangeScene();
	void SetEnemiesSpawnPositon();
	void SetInactivationByPosition();
	bool IsInViewport(LPGAMEOBJECT object);
};