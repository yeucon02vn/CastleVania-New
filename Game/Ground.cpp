#include "Ground.h"

Ground::Ground() : GameObject()
{
	AddAnimation("ground1_ani");
	AddAnimation("ground2_ani");
}

void Ground::Render()
{
	animations[state]->Render(nx,x, y);
}

void Ground::SetState(int state)
{
	GameObject::SetState(state);
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + GROUND_WIDTH;
	b = y + GROUND_HEIGHT;
}

