#pragma once
#include "Player.h"
#include "Map.h"
#include "SimpleGround.h"
#include "Lava.h"
#include "Drawer.h"
#include "Coin.h"
#include "Enemy1.h"
#include "Key.h"
#include "Door.h"
#include "bass.h"

struct UpdateCoords
{
	int x1, y1, x2, y2;
};

class Environment
{
public:
	Environment(Drawer*,Player*,const char*, int);
	void Cycle(Controls);
	void SetNonPlaybleObject(NonPlayble*);
	void SetMap(MapInfo);
	void NextMap();
	~Environment();
private:
	const int blockSize = 32, maxObj = 200;
	int currObjCount;
	NonPlayble** nonPlaybleObjects;
	Player* player;
	Drawer* drawer;
	Map* map;
	MapInfo mapInfo,firstMap;
	SimpleGround* simpleGround;
	Lava* lava;
	HSTREAM coinSound;
	DynObjInfo coin;
	int g;
	bool prevPlayerState;
	void ObjectCreation();
	void CollisionX(IDynamicObject*);
	void CollisionY(IDynamicObject*);
	UpdateCoords GetCoordsForUpdate(CoordAndSize);
};

