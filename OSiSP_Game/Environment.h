#pragma once
#include "Player.h"
#include "Map.h"
#include "SimpleGround.h"

struct UpdateCoords
{
	int x1, y1, x2, y2;
};

class Environment
{
public:
	Environment(Player*,MapInfo, int);
	void Cycle(Controls);
	~Environment();
private:
	const int blockSize = 32;
	Player* player;
	MapInfo mapInfo;
	SimpleGround* simpleGround;
	int g;
	DWORD time, prevTime;
	void CollisionX(IDynamicObject*);
	void CollisionY(IDynamicObject*);
	UpdateCoords GetCoordsForUpdate(CoordAndSize);
};

