
#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include <fstream>
#include <vector>
//#include "Items.h"
#include "Simon.h"
#include "Game.h"
#include "TileMap.h"
#include "UI.h"
#include "SubWeapon.h"

static int score;



class GameStage
{
	float simonDeadTime = 1.5;
protected:
	bool changeStage;
	float time;

	bool canUseKeyboard;
	static Simon* simon;
	SubWeapon* weapon;
	float timeKillAll;
	bool choose;
	static Game *game;
//	static UI* ui;
	static int Countdown;;
public:
	int stage;
	GameStage() { weapon = NULL; }
	~GameStage() {}
	virtual void Render() {};
	virtual void DestroyAll() = 0;


	virtual void Update(DWORD gameTime) {};
	virtual bool GetChangingStage() { return changeStage; };
	virtual void SetChangingStage(bool status) { changeStage = status; }

	void setGame(Game* game) { this->game = game; }

};
#endif __GAMESTATE_H__
