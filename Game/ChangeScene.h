#pragma once
#include "GameObject.h"
#include "Define.h"

class ChangeScene : public GameObject
{
	int idScene;

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false) {}
	virtual void Render() { RenderBoundingBox(); }

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetIdScene(int idnextscene) { this->idScene = idnextscene; }
	int GetIdScene() { return this->idScene; }
};

typedef ChangeScene * LPCHANGESCENE;

