#include "Door.h"


Door::Door() : GameObject()
{
	AddAnimation("door1_ani");
	AddAnimation("door2_idle_ani");
	AddAnimation("door2_open_ani");
	startTime = 0;
}

void Door::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

}

void Door::Render()
{

	animations[state]->Render(-1, x, y);
	if (state == DOOR2_OPEN)
		if (animations[state]->getCurrentFrame() == animations[state]->getLastFrame())
			state = DOOR2_IDLE;

}

void Door::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	switch (state)
	{
	case DOOR1:
		l = 1376;
		t = 272;
		r = l + DOOR_BBOX_WIDTH;
		b = t + DOOR_BBOX_HEIGHT;
		break;
	case DOOR2_IDLE:
	case DOOR2_OPEN:
		l = x - 16;
		t = y;
		r = l + DOOR_BBOX_WIDTH;
		b = t + DOOR_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}