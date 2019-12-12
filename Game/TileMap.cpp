#include "TileMap.h"

TileMap::TileMap(int framewidht, int frameheight)
{
	FrameHeight = framewidht;
	FrameWidth = frameheight;
	fit = 130;
	RowMatrix = 0;
	ScreenRow = 0;
	ColumnMatrix = 0;
	maxWidthRenderCam = 0;

}

TileMap::~TileMap()
{

}

void TileMap::ReadMap(LPCWSTR filename)
{
	RowMatrix = 0;
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

void TileMap::LoadResource()
{
	textures = Textures::GetInstance();
	sprites = Sprites::GetInstance();


	textures->Add(ID_TEX_LEVEL_ONE, L"Scenes\\Scene1.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 state1 = textures->Get(ID_TEX_LEVEL_ONE);
		for (int i = 0; i < 49; i++)
			sprites->Add("TileMap1_" + to_string(i), i * 32, 0, 32 + i * 32, 32, state1);

	textures->Add(ID_TEX_LEVEL_TWO, L"Scenes\\Scene2.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 state2 = textures->Get(ID_TEX_LEVEL_TWO);
	for (int i = 0; i < 102; i++)
		sprites->Add("TileMap2_" + to_string(i), i * 32, 0, 32 + i * 32, 32, state2);

	textures->Add(ID_TEX_LEVEL_THREE, L"Scenes\\Scene3.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 state3 = textures->Get(ID_TEX_LEVEL_THREE);
	for (int i = 0; i < 28; i++)
		sprites->Add("TileMap3_" + to_string(i), i * 32, 0, 32 + i * 32, 32, state3);
}


void TileMap::LoadMap(int idscene)
{
	minWidthRenderCam = 0;
	bool isChangeScene = true;
	idScene = idscene;
	switch (idscene)
	{
	case SCENE1 :
	{
		ScreenRow = MAP_HEIGHT_LV1 / FrameHeight;
		ColumnMatrix = MAP_WIDTH_LV1 / FrameWidth;
		maxWidthRenderCam = MAP_WIDTH_LV1;

		ReadMap(L"Scenes\\Scene1.txt");
		animation = new Animation(0);
		for (int i = 0; i < 49; i++)
			animation->Add("TileMap1_" + to_string(i));
		break;

	}
	case SCENE2:
	{
		
		ScreenRow = MAP_HEIGHT_LV2 / FrameHeight;
		ColumnMatrix = MAP_WIDTH_LV2 / FrameWidth;
		maxWidthRenderCam = 3072;

		ReadMap(L"Scenes\\Scene2.txt");
		animation = new Animation(0);
		for (int i = 0; i < 102; i++)
			animation->Add("TileMap2_" + to_string(i));
		break;
	}
	case SCENE2_1:
	case SCENE2_2:
	case SCENE2_3:
	{
		minWidthRenderCam = 3056;
		maxWidthRenderCam = 4096;
		//if (isChangeScene)
		//{
		//	ScreenRow = MAP_HEIGHT_LV2 / FrameHeight;
		//	ColumnMatrix = MAP_WIDTH_LV2 / FrameWidth;

		//	ReadMap(L"Scenes\\Scene2.txt");
		//	animation = new Animation(0);
		//	for (int i = 0; i < 102; i++)
		//		animation->Add("TileMap2_" + to_string(i));
		//}
		break;
	}
	case SCENE2_BOSS:
	{
		//if (isChangeScene)
		//{
		//	ScreenRow = MAP_HEIGHT_LV2 / FrameHeight;
		//	ColumnMatrix = MAP_WIDTH_LV2 / FrameWidth;

		//	ReadMap(L"Scenes\\Scene2.txt");
		//	animation = new Animation(0);
		//	for (int i = 0; i < 102; i++)
		//		animation->Add("TileMap2_" + to_string(i));
		//}
		break;
	}
	case SCENE3_1:
	case SCENE3_2:
	{
		ScreenRow = MAP_HEIGHT_LV3 / FrameHeight;
		ColumnMatrix = (MAP_WIDTH_LV3 + 64) / FrameWidth;
		maxWidthRenderCam = MAP_WIDTH_LV3;

		ReadMap(L"Scenes\\Scene3.txt");
		animation = new Animation(0);
		for (int i = 0; i < 28; i++)
			animation->Add("TileMap3_" + to_string(i));
		break;
	}
	default:
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

