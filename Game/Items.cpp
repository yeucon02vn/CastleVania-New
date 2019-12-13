#include "Items.h"

#include "Whip.h"

Items::Items() : GameObject()
{

	timeDelete = -1;
	// thêm các hình ảnh từ file
	AddAnimation("item_small_heart_ani");
	AddAnimation("item_big_heart_ani");
	AddAnimation("item_whip_ani");
	AddAnimation("item_knife_ani");


}

void Items::Render()
{
	int alpha = 255;

	if (GetTickCount() - timeDelete > ITEM_TIME_DESTROYED / 2)
	{
		alpha -= 100 * (rand() % 2);
	}
	animations[state]->Render(nx, x, y, alpha);
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	if (timeDelete == -1)
	{
		timeDelete = GetTickCount();
	}
	else
	{
		DWORD now = GetTickCount();

		if (now - timeDelete > ITEM_TIME_DESTROYED)
		{
			isDestroy = true;
			return;
		}
	}
	

	
	//if (state == ITEM_SMALL_HEART && vx_Itemfalling != 0)
	//{
	//	OutputDebugStringW(L"a");
	//	vx += vx_Itemfalling;
	//	if (vx >= ITEM_LIMITED_X || vx <= -ITEM_LIMITED_X)
	//		vx_Itemfalling *= -1; // đổi chiều
	//}

	GameObject::Update(dt);
	// Check collision between item and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// collision of Simon and Candle -> do nothing -> update x;
			if (dynamic_cast<Ground *>(e->obj))
			{
				y += min_ty * dy + ny * 0.4f;

				if (nx != 0) vx = 0;

				if (ny != 0)
				{
					
					vy = 0;
				}
				vx_Itemfalling = 0;
			}
			else
			{
				if (e->nx != 0)
					x += dx;
				if (e->ny != 0)
					y += dy;
			}
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

int Items::Random()
{
	int percent = rand() % 100; // random theo tỉ lệ
	if (percent < 75)
		RandomItem();
	else
		RandomSubWeapon();
	return state;
}

void Items::RandomItem()
{
	int percent = rand() % 100; // random theo tỉ lệ

	if (percent < 60)
	{
		if (checkMaxLevelWhip == false)
		{
			SetState(ITEM_WHIP);
		}
		else
		{
			SetState(ITEM_SMALL_HEART);
		}
	}
	else if (percent < 70)
	{
		SetState(ITEM_BIG_HEART);
	}
	else
	{
		RandomSubWeapon();
	}
}

void Items::RandomSubWeapon()
{
	//int percent = rand() % 100;
	//if (percent < 40)
	//{
	SetState(ITEM_KNIFE);
	//}

}

void Items::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case ITEM_BIG_HEART:
		right = x + BIG_HEART_BBOX_WIDTH;
		bottom = y + BIG_HEART_BBOX_HEIGHT;
		break;
	case ITEM_SMALL_HEART:
		right = x + SMALL_HEART_BBOX_WIDTH;
		bottom = y + SMALL_HEART_BBOX_HEIGHT;
		break;
	case ITEM_WHIP:
		right = x + MORNING_STAR_BBOX_WIDTH;
		bottom = y + MORNING_STAR_BBOX_HEIGHT;
		break;
	case ITEM_KNIFE:
		right = x + WEAPON_KNIFE_BBOX_WIDTH;
		bottom = y + WEAPON_KNIFE_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}

void Items::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ITEM_SMALL_HEART:
		vx_Itemfalling = ITEM_FALLING_SPEED_X;
		vx = 0;
		vy = ITEM_FALLING_SPEED_Y;
		break;
	default:
		vx = 0;
		vy = ITEM_FALLING_SPEED_Y;
		break;
	}
}
