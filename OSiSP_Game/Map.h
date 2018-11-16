#pragma once
#include <atlstr.h>

struct SpriteInfo
{
	char code;
	int spaceX, spaceY;
	char* pathToImg;
};

struct MapInfo
{
	int iCount, jCount, spritesCount;
	char** Map;
	char* backgroundPath;
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
	char** CreateCopyOfCodedMap(MapInfo mapInfo);
};

