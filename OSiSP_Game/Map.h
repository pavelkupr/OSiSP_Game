#pragma once
//#include <windows.h>
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
	
	~Map();
private:
	int i, j, spritesCount;
	char** codedMap;
	SpriteInfo* spritesInfo;
	char* pathToBackground;
	void LoadSprites(char* path);
	char** CreateCopyOfCodedMap();
};

