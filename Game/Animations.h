#pragma once
#include "Sprites.h"
class AnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	AnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return this->time; }
	LPSPRITE GetSprite() { return this->sprite; }
};

typedef AnimationFrame *LPANIMATION_FRAME;

class Animation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(string spriteId, DWORD time = 0);
	void setCurrentFrame(int frame);
	void Reset() { currentFrame = -1; }
	int getCurrentFrame();
	int getLastFrame();
	void DrawBoard(float x, float y, int alpha = 255);
	//void Render(float x, float y, int alpha = 255);
	void Render(int nx, float x, float y, int alpha = 255);
	void Draw(int x, int y);

};

typedef Animation *LPANIMATION;

class Animations
{
	static Animations * __instance;

	unordered_map<string, LPANIMATION> animations;

public:
	void Add(string id, LPANIMATION ani);
	LPANIMATION Get(string id);

	static Animations * GetInstance();
};