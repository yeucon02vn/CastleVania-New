#include "Animations.h"
#include "Game.h"
#include "debug.h"

void Animation::Add(string spriteId, DWORD time)
{
	if (time == 0) time = this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new AnimationFrame(sprite, time);
	frames.push_back(frame);
}



void Animation::Render(int nx, float x, float y, int alpha)
{
	DWORD now = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime >= t) //  
		{
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}

	}
	frames[currentFrame]->GetSprite()->Draw(nx,x, y, alpha);
}


void Animation::setCurrentFrame(int frame)
{
	this->currentFrame = frame;
}

int Animation::getCurrentFrame()
{
	return currentFrame;
}

int Animation::getLastFrame()
{
	return frames.size() - 1;
}

void Animation::DrawBoard(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime >= t)
		{
			currentFrame++;
			lastFrameTime = now;


			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}

	}
	frames[currentFrame]->GetSprite()->DrawBoard(x, y, alpha);
}

void Animation::Draw(int x, int y)
{
	frames[currentFrame]->GetSprite()->Draw(-1,x, y, 255);
}


Animations * Animations::__instance = NULL;

Animations * Animations::GetInstance()
{
	if (__instance == NULL) __instance = new Animations();
	return __instance;
}

void Animations::Add(string id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION Animations::Get(string id)
{
	return animations[id];
}

