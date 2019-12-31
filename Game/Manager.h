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
#include "Boss.h"
#include "MapObjectManager.h"
#include "UI.h"

using namespace std;
class Manager {
	int idScene;
	Grid * grid;
	Game *game;
	vector<LPGAMEOBJECT> listGetObject;
	vector<LPGAMEOBJECT> listGridObjects;
	vector<LPITEMS> listItems;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listDoors; 
	vector<LPGAMEOBJECT> listStaticObjects;
	vector<LPGAMEOBJECT> listMovingObjects;
	vector<LPGAMEOBJECT> listEffects;
	D3DXVECTOR2 pos;
	Simon * simon = NULL;
	SubWeapon * weapon;
	vector<SubWeapon*> listWeapon;
	TileMap * tileMap;
	MapsObjectsManager * mapsObjects;
	Water * water;
	Boss * boss;
	UI * ui;
	bool canControl;

	bool isBossFighting = false;
	bool isSetSimonAutoWalk = false;	
	bool isMovingCamera1 = false;
	bool isMovingCamera2 = false;
	int countDxCamera = 0;
public:
	Manager(Game * game);
	~Manager();
	void Init(int idScene);
	void LoadObjects(int id_scene);
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
	void Simon_Stair_Down();
	void Simon_Stair_Up();
	bool Simon_Stair_Stand();
	bool SimonWalkThroughDoor(); 
	bool CheckSimonCollisionStair();
	void UpdateCam();
	void ResetGame();
	void CrossEffect();
};