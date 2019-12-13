#pragma once
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Textures.h"
#include "Sprites.h"
class Object {
public:
	int idObject;
	float x;
	float y;
	int state;
	void Add(int id, float pos_x, float pos_y, int state) { idObject = id; x = pos_x; y = pos_y; this->state = state; }
	~Object() {}
	Object() {}
};

class ObjectsManager {
	int ID;
	vector<Object> mapObject;
public:
	ObjectsManager(int id) { this->ID = id; }
	void LoadObjectFromFile(LPCWSTR path);
	//Object getMapObject() { return mapObject; }
	vector<Object> getMapobject() { return mapObject; }
};
typedef ObjectsManager * LPObjectsManager;

class MapsObjectsManager {
	static MapsObjectsManager * _instance;
	unordered_map<int, LPObjectsManager> mapsObjects;
public:
	void Add(int ID, LPCWSTR filePath_data);
	LPObjectsManager Get(int ID) { return mapsObjects[ID]; }

	static MapsObjectsManager * GetInstance();
};