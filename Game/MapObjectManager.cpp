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
	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state;
		obj.Add(ID_Obj, pos_x, pos_y, state);
		mapObject.push_back(obj);
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
