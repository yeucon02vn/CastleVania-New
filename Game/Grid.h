#pragma once

#include <vector>
#include "GameObject.h"
#include "Enemy.h"
#include <unordered_set>
#include "Candle.h"
#include "Boss.h"
#include "Enemy.h"
#include "Fireball.h"
//class Grid;

//class Unit
//{
//	friend class Grid;
//private:
//	Grid * grid;
//	LPGAMEOBJECT obj;
//	float x;
//	float y;
//
//	Unit * prev;
//	Unit * next;
//
//public:
//	Unit(Grid* grid, LPGAMEOBJECT obj, float x, float y);
//	void Move(float x, float y);
//	LPGAMEOBJECT GetObj() { return this->obj; }
//};

class Cell{
	vector<LPGAMEOBJECT> listObject;
public:
	int cell_width_id;
	int cell_height_id;
	void Add(LPGAMEOBJECT obj, int cell_w_id, int cell_h_id);
	void DeleteObj(int i);
	void GetObjects(vector<LPGAMEOBJECT> * &objects);
};



class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int nums_col;
	int nums_row;
	static Grid * __instance;
	vector<vector<Cell*>> cells;

public:
	Grid(int map_width, int map_height, int cell_width = 256, int cell_height = 240);

	void Add(LPGAMEOBJECT obj, int cell_x, int cell_y);
	void Move(D3DXVECTOR2 camPosition);
	void Get(D3DXVECTOR2 camPosition,vector<LPGAMEOBJECT>& objects);
	static Grid * GetInstance(bool isReset, int map_width, int map_height);
	void Delete(D3DXVECTOR2 camPosition);
};

