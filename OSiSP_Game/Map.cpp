#include "Map.h"


Map::Map()
{
	i = 0;
	j = 0;
}

void Map::LoadMap(const char* path)
{
	FILE *file;
	char* pathToSprites;
	int pathLength,pathToBackgroundLength;
	file = fopen(path, "r");
	fscanf(file, "i: %d\n", &i);
	fscanf(file, "j: %d\n", &j);
	codedMap = (char**)malloc(sizeof(char*) * i);
	for (int x = 0; x < i; x++)
	{
		codedMap[x] = (char*)malloc(sizeof(char*) * j);
		for (int y = 0; y < j; y++)
		{
			fscanf(file, "%c", &codedMap[x][y]);
		}
		fscanf(file, "\n");
	}
	fscanf(file, "Path length: %d\n", &pathLength);
	pathToSprites = (char*)malloc(sizeof(char*)*(pathLength + 1));
	fscanf(file, "Path to sprites: %s\n", pathToSprites);
	fscanf(file, "Path length to background: %d\n", &pathToBackgroundLength);
	pathToBackground = (char*)malloc(sizeof(char*)*(pathToBackgroundLength + 1));
	fscanf(file, "Path to background: %s\n", pathToBackground);
	fclose(file);
	LoadSprites(pathToSprites);
}

MapInfo Map::GetMapInfo()
{
	MapInfo result;
	result.iCount = i;
	result.jCount = j;
	result.spritesCount = spritesCount;
	result.Map = CreateCopyOfCodedMap();
	result.spritesInfo = (SpriteInfo*)malloc(sizeof(SpriteInfo) * spritesCount);
	result.backgroundPath = pathToBackground;
	for (int x = 0; x < spritesCount; x++)
		result.spritesInfo[x] = spritesInfo[x];
	return result;
}

char** Map::CreateCopyOfCodedMap()
{
	char** result = (char**)malloc(sizeof(char*) * i);
	for (int x = 0; x < i; x++)
	{
		result[x] = (char*)malloc(sizeof(char*) * j);
		for (int y = 0; y < j; y++)
		{
			result[x][y]=codedMap[x][y];
		}
	}
	return result;
}

void Map::LoadSprites(char* path)
{
	FILE *file;
	int pathLength;
	file = fopen(path, "r");
	fscanf(file, "CountOfSprites: %d\n", &spritesCount);
	spritesInfo = (SpriteInfo*)malloc(sizeof(SpriteInfo) * spritesCount);

	for (int x = 0; x < Map::spritesCount; x++)
	{
		fscanf(file, "Code: %c\n", &spritesInfo[x].code);
		fscanf(file, "SpaceX: %d\n", &spritesInfo[x].spaceX);
		fscanf(file, "SpaceY: %d\n", &spritesInfo[x].spaceY);
		fscanf(file, "Path length: %d\n", &pathLength);
		spritesInfo[x].pathToImg = (char*)malloc(sizeof(char*)*(pathLength + 1));
		fscanf(file, "Path: %s\n", spritesInfo[x].pathToImg);
	}
	fclose(file);
}

Map::~Map()
{
	if (i != 0 || j != 0)
	{
		for (int x = 0; x < i; x++)
		{
			delete codedMap[x];
		}
		delete codedMap;
		delete spritesInfo;
	}
}
