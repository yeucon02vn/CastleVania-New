#include "TileMap.h"

TileMap::TileMap()
{
	
}

TileMap::~TileMap()
{

}

void TileMap::ReadMap(LPCWSTR filename)
{
	ifstream file;
	file.open(filename, ios::in);


	while (!file.eof())
	{
		for (int j = 0; j < ColumnMatrix; j++)
		{
			file >> Tile_Map[RowMatrix][j];
		}
		RowMatrix++;
	}
}

void TileMap::LoadMap(int state)
{
	textures = Textures::GetInstance();
	sprites = Sprites::GetInstance();
	switch (state)
	{
	case SCENE1:
	{
		ReadMap(L"Scenes\\Scene1.txt");

		textures->Add(ID_TEX_LEVEL_ONE, L"Scenes\\Scene1.png", D3DCOLOR_XRGB(255, 255, 255));

		LPDIRECT3DTEXTURE9 state1 = textures->Get(ID_TEX_LEVEL_ONE);
		for (int i = 0; i < 49; i++)
			sprites->Add("TileMap1_" + to_string(i), i * 32, 0, 32 + i * 32, 32, state1);

		animation = new Animation(0);
		for (int i = 0; i < 49; i++)
			animation->Add("TileMap1_" + to_string(i));
	
		break;
	}
	case SCENE2:
		ReadMap(L"Scenes\\Scene2.txt");

		textures->Add(ID_TEX_LEVEL_TWO, L"Scenes\\Scene2.png", D3DCOLOR_XRGB(255, 255, 255));

		LPDIRECT3DTEXTURE9 state2 = textures->Get(ID_TEX_LEVEL_TWO);
		for (int i = 0; i < 102; i++)
			sprites->Add("TileMap2_" + to_string(i), i * 32, 0, 32 + i * 32, 32, state2);

		animation = new Animation(0);
		for (int i = 0; i < 102; i++)
			animation->Add("TileMap2_" + to_string(i));
		break;
	}



}



//Vẽ lại hàm drawmap(Updated)
void TileMap::DrawMap(D3DXVECTOR2 cam)
{

	int colStart = cam.x / FrameWidth;
	int colEnd = ((cam.x + SCREEN_WIDTH) / FrameWidth + 2);

	for (int i = 0; i < ScreenRow; i++)
	{
		for (int j = colStart; j < colEnd; j++)
		{
			x = FrameWidth * (j - colStart) + cam.x - (int)cam.x % 32;
			y = i * FrameHeight + fit;
			animation->setCurrentFrame(Tile_Map[i][j]);
			animation->Draw(x, y);
		}
	}
}

void TileMap::InitMap(int state, int mapWidth, int mapHeight, int frameHeight, int frameWidth)
{
	FrameHeight = frameHeight;
	FrameWidth = frameWidth;
	ScreenRow = (mapHeight / FrameHeight);
	ColumnMatrix = mapWidth / FrameWidth;
	LoadMap(state);
	RowMatrix = 0;
	fit = 130;
}

