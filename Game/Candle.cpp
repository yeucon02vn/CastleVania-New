#include "Candle.h"

Candle::Candle() : GameObject()
{
	AddAnimation("bigcandle_ani");
	AddAnimation("smallcandle_ani");
}


void Candle::Render()
{
	animations[state]->Render(nx,x,y );
}


void Candle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	switch (state)
	{
	case ANI_BIG_CANDLE:
		r = x + LCANDLE_WIDTH;
		b = y + LCANDLE_HEIGHT;
	case ANI_SMALL_CANDLE:
		r = x + MCANDLE_WIDTH;
		b = y + MCANDLE_HEIGHT;
	}
}

void Candle::SetState(int state)
{
	GameObject::SetState(state);
}
