#include "MapObjectManager.h"

void ObjectsManager::LoadObjectFromFile(LPCWSTR path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", ID, path);
		fs.close();
		return;
	}

	Object obj;
	int ID_Obj;
	float pos_x, pos_y;
	int state;
	bool isDropItem;

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isDropItem;
		obj.Add(ID_Obj, pos_x, pos_y, state, isDropItem, int(pos_x/256), int(pos_y/240));
		mapObject.push_back(obj);
	}

}

void MapsObjectsManager::LoadObjectManagerFromFile(LPCWSTR path)
{
	fstream fs;
	fs.open(path, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", path);
		fs.close();
		return;
	}
	int ID_MapObj;
	string path_data;
	while (!fs.eof())
	{
		fs >> ID_MapObj >> path_data;
		wstring path_object(path_data.begin(), path_data.end());
		Add(ID_MapObj, path_object.c_str());
	}

}

MapsObjectsManager * MapsObjectsManager::_instance = NULL;

void MapsObjectsManager::Add(int ID, LPCWSTR filePath_data)
{
	LPObjectsManager mapObject = new ObjectsManager(ID);
	mapObject->LoadObjectFromFile(filePath_data);
	mapsObjects[ID] = mapObject;
}

MapsObjectsManager * MapsObjectsManager::GetInstance()
{
	if (_instance == NULL) _instance = new MapsObjectsManager();
	return _instance;
}

