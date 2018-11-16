#include "Map.h"


Map::Map()
{
	mapInfo.iCount = 0;
	mapInfo.jCount = 0;
}

void Map::LoadMap(const char* path)
{
	FILE *file;
	char* pathToSprites;
	int pathLen,pathToBackLen;
	file = fopen(path, "r");
	fscanf(file, "i: %d\n", &mapInfo.iCount);
	fscanf(file, "j: %d\n", &mapInfo.jCount);

	//Проверка на некорректный файл
	if (mapInfo.iCount <= 0 || mapInfo.jCount <= 0)
	{
		fclose(file);
		mapInfo.iCount = 0;
		mapInfo.jCount = 0;
		return;
	}

	mapInfo.Map = (char**)malloc(sizeof(char*) * mapInfo.iCount);
	for (int x = 0; x < mapInfo.iCount; x++)
	{
		mapInfo.Map[x] = (char*)malloc(sizeof(char*) * mapInfo.jCount);
		for (int y = 0; y < mapInfo.jCount; y++)
		{
			fscanf(file, "%c", &mapInfo.Map[x][y]);
		}
		fscanf(file, "\n");
	}

	fscanf(file, "Path length: %d\n", &pathLen);
	pathToSprites = (char*)malloc(sizeof(char*)*(pathLen + 1));
	fscanf(file, "Path to sprites: %s\n", pathToSprites);
	fscanf(file, "Path length to background: %d\n", &pathToBackLen);
	mapInfo.backgroundPath = (char*)malloc(sizeof(char*)*(pathToBackLen + 1));
	fscanf(file, "Path to background: %s\n", mapInfo.backgroundPath);
	fclose(file);
	mapInfo.spritesCount = LoadSprites(pathToSprites, &mapInfo.spritesInfo);
	delete pathToSprites;
}

MapInfo Map::GetMapInfo()
{
	MapInfo result;
	result.iCount = mapInfo.iCount;
	result.jCount = mapInfo.jCount;
	result.spritesCount = mapInfo.spritesCount;
	result.backgroundPath = (char*)malloc(sizeof(char*) * strlen(mapInfo.backgroundPath));
	strcpy(result.backgroundPath,mapInfo.backgroundPath);
	result.Map = CreateCopyOfCodedMap(mapInfo);
	result.spritesInfo = (SpriteInfo*)malloc(sizeof(SpriteInfo) * result.spritesCount);
	for (int i = 0; i < result.spritesCount; i++)
	{
		result.spritesInfo[i].code = mapInfo.spritesInfo[i].code;
		result.spritesInfo[i].spaceX = mapInfo.spritesInfo[i].spaceX;
		result.spritesInfo[i].spaceY = mapInfo.spritesInfo[i].spaceY;
		result.spritesInfo[i].pathToImg = (char*)malloc(sizeof(char*) * strlen(mapInfo.spritesInfo[i].pathToImg));
		strcpy(result.spritesInfo[i].pathToImg, mapInfo.spritesInfo[i].pathToImg);
	}
	return result;
}

char** Map::CreateCopyOfCodedMap(MapInfo mapInfo)
{
	char** result = (char**)malloc(sizeof(char*) * mapInfo.iCount);
	for (int x = 0; x < mapInfo.iCount; x++)
	{
		result[x] = (char*)malloc(sizeof(char*) * mapInfo.jCount);
		for (int y = 0; y < mapInfo.jCount; y++)
		{
			result[x][y]= mapInfo.Map[x][y];
		}
	}
	return result;
}

int Map::LoadSprites(char* path, SpriteInfo** spritesInfo)
{
	FILE *file;
	int pathLength, count;
	SpriteInfo* info;
	file = fopen(path, "r");
	fscanf(file, "CountOfSprites: %d\n", &count);
	info = *spritesInfo = (SpriteInfo*)malloc(sizeof(SpriteInfo) * count);

	for (int x = 0; x < count; x++)
	{
		fscanf(file, "Code: %c\n", &info[x].code);
		fscanf(file, "SpaceX: %d\n", &info[x].spaceX);
		fscanf(file, "SpaceY: %d\n", &info[x].spaceY);
		fscanf(file, "Path length: %d\n", &pathLength);
		info[x].pathToImg = (char*)malloc(sizeof(char*)*(pathLength + 1));
		fscanf(file, "Path: %s\n", info[x].pathToImg);
	}
	fclose(file);
	return count;
}

void Map::DeleteMapInfo(MapInfo mapInfo)
{
	if (mapInfo.iCount > 0 && mapInfo.jCount > 0)
	{
		for (int x = 0; x < mapInfo.iCount; x++)
		{
			delete mapInfo.Map[x];
		}
		delete mapInfo.Map;

		for (int x = 0; x < mapInfo.spritesCount; x++)
		{
			delete mapInfo.spritesInfo[x].pathToImg;
		}
		delete mapInfo.spritesInfo;

		delete mapInfo.backgroundPath;
	}
}

Map::~Map()
{
	DeleteMapInfo(mapInfo);
}
