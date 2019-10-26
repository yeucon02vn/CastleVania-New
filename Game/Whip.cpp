#include "Whip.h"


Whip::Whip() : GameObject()
{
	AddAnimation("normalwhip_ani");
	AddAnimation("shortchain_ani");
	AddAnimation("longchain_ani");
	SetState(WHIP_NORMAL);
}

void Whip::Update()
{

}

void Whip::Render()
{
	animations[state]->Render(nx,x, y);
}

void Whip::SetWhipPosition(float x, float y, bool isSit)
{

	if (isSit == true)
		y += 15.0f;

	x -= 90.0f;
	y += 1.0f;

	SetPosition(x, y);


}

void Whip::UpLevelWhip()
{
	if (state == WHIP_NORMAL) 
		state = WHIP_SHORT; 
	else if (state == WHIP_SHORT)
	{
		state = WHIP_LONG;
		isMaxLevel = true;
	}
}


bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	return AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (isMaxLevel)
	{
		if (nx < 0)
		{
			left = x + 20;
			right = left + WHIP_BBOX_WIDTH + 30;
		}
		else if (nx > 0)
		{
			left = x + 150;
			right = left + WHIP_BBOX_WIDTH + 30;
		}
	}
	else
	{
		if (nx < 0)
		{
			left = x + 50;
			right = left + WHIP_BBOX_WIDTH;
		}
		else if (nx > 0)
		{
			left = x + 150;
			right = left + WHIP_BBOX_WIDTH;
		}
	}
}

void Whip::reset()
{
	animations[state]->Reset();
}

