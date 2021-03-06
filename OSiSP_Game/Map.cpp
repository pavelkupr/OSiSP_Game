#include "Map.h"

Map::Map()
{
	mapInfo.iCount = 0;
	mapInfo.jCount = 0;
}

void Map::LoadMap(const char* path)
{
	FILE *file;
	char* pathToSprites, *pathToObjs;
	int pathLen,pathToBackLen;
	DeleteMapInfo(mapInfo);
	file = fopen(path, "r");
	fscanf(file, "i: %d\n", &mapInfo.iCount);
	fscanf(file, "j: %d\n", &mapInfo.jCount);

	//�������� �� ������������ ����
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

	fscanf(file, "Path length: %d\n", &pathLen);
	pathToObjs = (char*)malloc(sizeof(char*)*(pathLen + 1));
	fscanf(file, "Path to objs: %s\n", pathToObjs);

	fscanf(file, "Path length: %d\n", &pathLen);
	mapInfo.pathToNext = (char*)malloc(sizeof(char*)*(pathLen + 1));
	fscanf(file, "Path to next: %s\n", mapInfo.pathToNext);

	fscanf(file, "Path length to background: %d\n", &pathToBackLen);
	mapInfo.backgroundPath = (char*)malloc(sizeof(char*)*(pathToBackLen + 1));
	fscanf(file, "Path to background: %s\n", mapInfo.backgroundPath);
	fclose(file);
	mapInfo.spritesCount = LoadSprites(pathToSprites, &mapInfo.spritesInfo);
	mapInfo.objsCount = LoadObjsInfo(pathToObjs, &mapInfo.objsInfo);
	delete pathToSprites;
	delete pathToObjs;
}

MapInfo Map::GetMapInfo()
{
	MapInfo result;
	result.iCount = mapInfo.iCount;
	result.jCount = mapInfo.jCount;
	result.spritesCount = mapInfo.spritesCount;
	result.objsCount = mapInfo.objsCount;
	result.backgroundPath = (char*)malloc(sizeof(char*) * strlen(mapInfo.backgroundPath));
	strcpy(result.backgroundPath,mapInfo.backgroundPath);
	result.pathToNext = (char*)malloc(sizeof(char*) * strlen(mapInfo.pathToNext));
	strcpy(result.pathToNext, mapInfo.pathToNext);
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
	result.objsInfo = (DynObjInfo*)malloc(sizeof(DynObjInfo) * result.objsCount);
	for (int i = 0; i < result.objsCount; i++)
	{
		result.objsInfo[i].code = mapInfo.objsInfo[i].code;
		result.objsInfo[i].mode = mapInfo.objsInfo[i].mode;
		result.objsInfo[i].frameCount = mapInfo.objsInfo[i].frameCount;
		result.objsInfo[i].coordsCount = mapInfo.objsInfo[i].coordsCount;
		result.objsInfo[i].coords =	(POINT*)malloc(sizeof(POINT) * result.objsInfo[i].coordsCount);
		for (int j = 0; j < result.objsInfo[i].coordsCount; j++)
		{
			result.objsInfo[i].coords[j].x = mapInfo.objsInfo[i].coords[j].x;
			result.objsInfo[i].coords[j].y = mapInfo.objsInfo[i].coords[j].y;
		}
		result.objsInfo[i].pathToImg = (char*)malloc(sizeof(char*) * strlen(mapInfo.objsInfo[i].pathToImg));
		strcpy(result.objsInfo[i].pathToImg, mapInfo.objsInfo[i].pathToImg);
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

int Map::LoadObjsInfo(char* path, DynObjInfo** spritesInfo)
{
	FILE *file;
	int pathLength, count, nameLength;
	DynObjInfo* info;
	file = fopen(path, "r");
	fscanf(file, "CountOfObjs: %d\n", &count);
	info = *spritesInfo = (DynObjInfo*)malloc(sizeof(DynObjInfo) * count);

	for (int x = 0; x < count; x++)
	{
		fscanf(file, "Code: %c\n", &info[x].code);
		fscanf(file, "Coords length: %d\n", &info[x].coordsCount);
		info[x].coords = (POINT*)malloc(sizeof(POINT)*(info[x].coordsCount));
		for (int i = 0; i < info[x].coordsCount; i++)
		{
			fscanf(file, "X: %d\n", &info[x].coords[i].x);
			fscanf(file, "Y: %d\n", &info[x].coords[i].y);
		}
		fscanf(file, "Frames: %d\n", &info[x].frameCount);
		fscanf(file, "Mode: %d\n", &info[x].mode);
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

		for (int x = 0; x < mapInfo.objsCount; x++)
		{
			delete mapInfo.objsInfo[x].pathToImg;
		}
		delete mapInfo.objsInfo;
		delete mapInfo.pathToNext;
		delete mapInfo.backgroundPath;
	}
}

Map::~Map()
{
	DeleteMapInfo(mapInfo);
}
