#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Game.h"
#include "debug.h"

using namespace std;

class Sprite
{
	string id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	int getWidth();
	int getHeight();
	void Draw(int nx, float x, float y, int alpha);
	//void Draw(float x, float y, int alpha = 255);
	void DrawBoard(float x, float y, int alpha = 255);
};

typedef Sprite * LPSPRITE;

/*
Manage sprite database
*/
class Sprites
{
	static Sprites * __instance;

	unordered_map<string, LPSPRITE> sprites;

public:
	void Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(string id);

	static Sprites * GetInstance();
};



