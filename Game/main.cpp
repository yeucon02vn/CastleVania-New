/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 04 - COLLISION

This sample illustrates how to:

1/ Implement SweptAABB algorithm between moving objects
2/ Implement a simple (yet effective) collision frame work

Key functions:
CGame::SweptAABB
CGameObject::SweptAABBEx
CGameObject::CalcPotentialCollisions
CGameObject::FilterCollision

CGameObject::GetBoundingBox

================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Define.h"
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "KeyEventHandler.h"
#include "StageManager.h"


Game *game;
StageManager* scene;
Textures * textures = Textures::GetInstance();
Sprites * sprites = Sprites::GetInstance();
Animations * animations = Animations::GetInstance();


void LoadSprites(int id, LPCWSTR tex, LPCWSTR sprite_data, LPCWSTR animation_data)
{
	textures->Add(id, tex , D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texture = textures->Get(id);

	ifstream spriteReader, animationReader;

	spriteReader.open(sprite_data);
	animationReader.open(animation_data);

	if (spriteReader.fail())
	{
		//DebugOut(L"[ERROR] LoadSprites failed!: ID=%d", id);
		spriteReader.close();
		return;
	}

	if (animationReader.fail())
	{
		//DebugOut(L"[ERROR] LoadAnimation failed!: ID=%d", id);
		animationReader.close();
		return;
	}

	// Load sprite data

	string spriteid;
	int left, top, right, bottom;

	while (spriteReader >> spriteid >> left >> top >> right >> bottom)
		sprites->Add(spriteid, left, top, right, bottom, texture);

	spriteReader.close();

	// Load animation data

	string animationId;
	string line;
	string spriteId;
	int frameTime;

	while (getline(animationReader, line))
	{
		LPANIMATION ani = new Animation();

		istringstream iss(line, istringstream::in);
		iss >> animationId;

		while (iss >> spriteId >> frameTime)
			ani->Add(spriteId, frameTime);

		animations->Add(animationId, ani);
	}

	animationReader.close();
}


void LoadResources()
{
	textures->Add(ID_TEX_BBOX, L"Textures\\BBox.png", D3DCOLOR_XRGB(255, 0, 255));
	LoadSprites(ID_TEX, L"Textures\\Castlevania.png", L"Textures\\Castlevania_sprites.txt", L"Textures\\Castlevania_animations.txt");
}



LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
Load all game resources
In this example: load textures, sprites, animations and mario object

TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/


/*
Update world status for this frame
dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	scene->Update(dt);

}

/*
Render a frame
*/



HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = WINDOW_CLASS_NAME;
	wcx.hIconSm = NULL;

	if (RegisterClassEx(&wcx) == 0)
		return false;

	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	HWND hWnd = CreateWindow(
		WINDOW_CLASS_NAME,
		MAIN_WINDOW_TITLE,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	if (!FULLSCREEN)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd,
			0,
			0,
			SCREEN_WIDTH + (SCREEN_WIDTH - clientRect.right),
			SCREEN_HEIGHT + (SCREEN_HEIGHT - clientRect.bottom),
			TRUE);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		scene->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);

}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= MAX_TIME_PER_FRAME)
		{
			frameStart = now;
			PollKeyboard();
			Update(dt);
			Render();
		}
		else
			Sleep(MAX_TIME_PER_FRAME - dt);
	}

	return 1;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	game = Game::GetInstance();
	game->Init(hWnd);
	LoadResources();
	InitDirectInput();
	InitKeyboard(hWnd);

	scene = new StageManager();
	scene->Init(game);
	scene->LoadStage(STAGE1);




	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}

