#pragma once
#include <Windows.h>
#include <atlstr.h>

struct SpriteInfo
{
	char code;
	int spaceX, spaceY;
	char* pathToImg;
};

struct DynObjInfo
{
	char code;
	POINT* coords;
	int mode;
	int frameCount;
	int coordsCount;
	char* pathToImg;
};

struct MapInfo
{
	int iCount, jCount, spritesCount,objsCount;
	char** Map;
	char* backgroundPath;
	char* pathToNext;
	DynObjInfo* objsInfo;
	SpriteInfo* spritesInfo;
};

class Map
{
public:
	Map();
	void LoadMap(const char* path);
	MapInfo GetMapInfo();
	void DeleteMapInfo(MapInfo mapInfo);
	~Map();

private:
	MapInfo mapInfo;
	int LoadSprites(char* path, SpriteInfo** spritesInfo);
	int LoadObjsInfo(char* path, DynObjInfo** spritesInfo);
	char** CreateCopyOfCodedMap(MapInfo mapInfo);
};

