#include "Grid.h"



Grid* Grid::__instance = NULL;

Grid *Grid::GetInstance(bool isReset, int map_width, int map_height)
{
	if (__instance == NULL || isReset == true) 
		__instance = new Grid(map_width, map_height);
	return __instance;
}

Grid::Grid(int map_width, int map_height, int cell_width, int cell_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->cell_width = cell_width;
	this->cell_height = cell_height;

	nums_col = map_width / cell_width;
	nums_row = map_height / cell_height;

	cells.resize(nums_row);
	for (UINT i = 0; i < cells.size(); ++i)
		cells[i].resize(nums_col, NULL);
}



void Grid::Add(LPGAMEOBJECT obj, int cell_x, int cell_y)
{
	if (cells[cell_y][cell_x] == NULL)
		cells[cell_y][cell_x] = new Cell();

	cells[cell_y][cell_x]->Add(obj,cell_x,cell_y);
}


void Grid::Move(D3DXVECTOR2 camPosition)
{
	// lấy chỉ số cell cũ

	int start_col = (int)(camPosition.x / cell_width);
	int end_col = ceil((camPosition.x + SCREEN_WIDTH) / cell_width);

	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			if (cells[i][j] == NULL)
				continue;

			// Get objects containing in cell
			vector<LPGAMEOBJECT> * cellObjects = NULL;
			cells[i][j]->GetObjects(cellObjects);

			// Get the unique pointer to the game object
			for (UINT k = 0; k < cellObjects->size(); k++)
				if (cellObjects->at(k)->isDestroy == false && dynamic_cast<Enemy*>(cellObjects->at(k)))
				{
					if (!dynamic_cast<Boss*>(cellObjects->at(k)))
					{
						int cell_x_new = cellObjects->at(k)->x / cell_width;
						int cell_y_new = cellObjects->at(k)->y / cell_height;
						if (cells[i][j]->cell_width_id != cell_x_new || cells[i][j]->cell_height_id != cell_y_new)
						{
							cells[cell_y_new][cell_x_new]->Add(cellObjects->at(k), cell_x_new, cell_y_new);
							cells[i][j]->DeleteObj(k);
						}
					}

				}
		}
	}
}

void Grid::Get(D3DXVECTOR2 camPosition, vector<LPGAMEOBJECT>& objects)
{
	int start_col = (int)(camPosition.x / cell_width);
	int end_col = ceil((camPosition.x + SCREEN_WIDTH) / cell_width);
	unordered_set<LPGAMEOBJECT> temp;
	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			if (cells[i][j] == NULL)
				continue;

			// Get objects containing in cell
			vector<LPGAMEOBJECT> * cellObjects = NULL;
			cells[i][j]->GetObjects(cellObjects);

			// Get the unique pointer to the game object
			for (UINT i = 0; i < cellObjects->size(); i++)
				temp.insert(cellObjects->at(i));
		}
	}
	for (auto it = temp.begin(); it != temp.end(); ++it)
		objects.push_back(*it);
}

void Grid::Delete(D3DXVECTOR2 camPosition)
{
	int start_col = (int)(camPosition.x / cell_width);
	int end_col = ceil((camPosition.x + SCREEN_WIDTH) / cell_width);
	int pos = -1;
	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			if (cells[i][j] == NULL)
				continue;
			// Get objects containing in cell
			vector<LPGAMEOBJECT> * cellObjects = NULL;
			cells[i][j]->GetObjects(cellObjects);

			// Get the unique pointer to the game object
			for (UINT i = 0; i < cellObjects->size(); i++)
				if ((dynamic_cast<FireBall*>(cellObjects->at(i)) || dynamic_cast<Candle*>(cellObjects->at(i)) || dynamic_cast<Boss*>(cellObjects->at(i))) && cellObjects->at(i)->isDestroy == true)
					pos = i;
			if (pos != -1)
			{
				cells[i][j]->DeleteObj(pos);
				pos = -1;
			}
		}
	}
}

void Cell::Add(LPGAMEOBJECT obj, int cell_w_id, int cell_h_id)
{
	listObject.push_back(obj);
	cell_width_id = cell_w_id; 
	cell_height_id = cell_h_id;
}

void Cell::DeleteObj(int i)
{
	listObject.erase(listObject.begin() + i);
}

void Cell::GetObjects(vector<LPGAMEOBJECT>*& objects)
{
	objects = &(this->listObject);
}
