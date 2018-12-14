#include "Environment.h"

Environment::Environment(Drawer* drawer, const char* map, int g)
{
	this->g = g;
	this->map = new Map();
	this->map->LoadMap(map);
	this->mapInfo = this->map->GetMapInfo();
	this->drawer = drawer;
	player = new Player(64, 64, (HBITMAP)LoadImage(NULL, TEXT("player.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), 5, 12, 8);
	coinSound = BASS_StreamCreateFile(FALSE, TEXT("coin.wav"), 0, 0, 0);
	firstMap = mapInfo;
	drawer->SetPlayer(player);
	player->SetG(g);
	prevPlayerState = true;
	currObjCount = 0;
	simpleGround = new SimpleGround();
	lava = new Lava();
	records = new Records();
	nonPlaybleObjects = (NonPlayble**)malloc(sizeof(NonPlayble**) * maxObj);
	for (int i = 0; i < maxObj; i++)
	{
		nonPlaybleObjects[i] = 0;
	}
	drawer->SetDrawInfo(mapInfo);
	drawer->SetNonPlaybleObjects(nonPlaybleObjects, &currObjCount);
	ObjectCreation();
}

void Environment::NextMap()
{
	char end[4] = "end";
	bool endGame = false;
	for(int i = 0; i <= 3; i++)
	{
		if (mapInfo.pathToNext[i] != end[i])
			break;
		if (i == 3)
			endGame = true;
			
	}
	if (endGame)
	{
		Record record;
		record.coins = player->GetCoinCount();
		record.name = player->name;
		records->SetNewRecord(record);
		player->RespawnOrDispose();
		mapInfo = firstMap;
		SetMap(mapInfo);
	}
	else
	{
		if (mapInfo.pathToNext != firstMap.pathToNext)
			map->DeleteMapInfo(mapInfo);
		player->SetCoord(64, 64);
		map->LoadMap(mapInfo.pathToNext);
		mapInfo = map->GetMapInfo();
		SetMap(mapInfo);
	}
}

void Environment::SetMap(MapInfo map)
{
	currObjCount = 0;
	this->mapInfo = map;
	drawer->SetDrawInfo(map);
	ObjectCreation();
}
void Environment::ObjectCreation()
{
	HANDLE curr;
	for (int i = 0; i < mapInfo.objsCount; i++)
	{
		if (mapInfo.objsInfo[i].code == 'C')
			coin = mapInfo.objsInfo[i];
		curr = LoadImage(NULL, CA2W(mapInfo.objsInfo[i].pathToImg), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		for(int j=0;j< mapInfo.objsInfo[i].coordsCount;j++)
		{
			switch (mapInfo.objsInfo[i].code)
			{
			case'C':
				SetNonPlaybleObject(new Coin(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount, mapInfo.objsInfo[i].mode, coinSound));
				break;
			case'E':
				if(mapInfo.objsInfo[i].mode >= 0)
					SetNonPlaybleObject(new Enemy1(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount,4, mapInfo.objsInfo[i].mode*blockSize));
				else
					SetNonPlaybleObject(new Enemy1(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount, 7, mapInfo.objsInfo[i].mode*blockSize));
				break;
			case'K':
				SetNonPlaybleObject(new Key(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount, mapInfo.objsInfo[i].mode));
				break;
			case'D':
				SetNonPlaybleObject(new Door(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize+4, (HBITMAP)curr, player));
				break;
			default:
				break;
			}
		}
	}
}

void Environment::Cycle(Controls controls)
{
	for (int i = 0; i < currObjCount; i++)
	{
		if (nonPlaybleObjects[i] != 0)
		{
			if (nonPlaybleObjects[i]->IsVisible())
			{
				nonPlaybleObjects[i]->Move();
				CollisionX(nonPlaybleObjects[i]);
				CollisionY(nonPlaybleObjects[i]);
			}

			if(!nonPlaybleObjects[i]->IsVisible())
			{
				delete nonPlaybleObjects[i];
				nonPlaybleObjects[i] = 0;
				for (int j = i + 1; j < currObjCount; j++)
				{
					nonPlaybleObjects[j - 1] = nonPlaybleObjects[j];
				}
				currObjCount--;
				i--;
			}
		}
	}

	if (player->IsVisible())
	{
		player->Move(controls);
		CollisionX(player);
		CollisionY(player);
		if (player->IsLive())
		{
			for (int i = 0; i < currObjCount; i++)
			{
				if (nonPlaybleObjects[i]->IsVisible())
				{
					if (player->GetCoordAndSize().x + player->GetCoordAndSize().width > nonPlaybleObjects[i]->GetCoordAndSize().x &&
						player->GetCoordAndSize().x < nonPlaybleObjects[i]->GetCoordAndSize().x + nonPlaybleObjects[i]->GetCoordAndSize().width)
						if (player->GetCoordAndSize().y + player->GetCoordAndSize().height > nonPlaybleObjects[i]->GetCoordAndSize().y &&
							player->GetCoordAndSize().y < nonPlaybleObjects[i]->GetCoordAndSize().y + nonPlaybleObjects[i]->GetCoordAndSize().height)
							nonPlaybleObjects[i]->Interact(player);
				}
			}
		}
	}

	if(!prevPlayerState && player->IsLive())
	{
		SetMap(firstMap);
	}

	if (player->GetIsNext())
		NextMap();

	if (player->GetIsHit())
	{
		int coins = player->GetCoinCount();
		if (coins > 0)
		{
			HANDLE curr = LoadImage(NULL, CA2W(coin.pathToImg), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			for (int i = 0; i < 5 && coins > 0; i++)
			{
				SetNonPlaybleObject(new Coin(player->GetCoordAndSize().x,
					player->GetCoordAndSize().y, (HBITMAP)curr, coin.frameCount, 1, coinSound));
				coins--;
				player->SetCoins(-1);
			}
		}
	}

	prevPlayerState = player->IsLive();
}

void Environment::CollisionX(IDynamicObject* dynamicObject)
{
	PrevMovement prevMovement = dynamicObject->GetPrevMovement();
	dynamicObject->SetCoord(dynamicObject->GetCoordAndSize().x, dynamicObject->GetCoordAndSize().y - prevMovement.y);
	UpdateCoords currInfo = GetCoordsForUpdate(dynamicObject->GetCoordAndSize());

	for (int i = currInfo.y1; i <= currInfo.y2; i++)
		for (int j = currInfo.x1; j <= currInfo.x2; j++)
		{
			if((i < mapInfo.iCount && j < mapInfo.jCount) && (i >= 0 && j >= 0))
				switch (mapInfo.Map[i][j])
				{
				case 'B':
					simpleGround->SetCoord(j*blockSize,i*blockSize);
					simpleGround->InteractX(dynamicObject);
					break;
				case 'L':
					lava->InteractX(dynamicObject);
					break;
				default:
					break;
				}
		}
	dynamicObject->SetCoord(dynamicObject->GetCoordAndSize().x, dynamicObject->GetCoordAndSize().y + prevMovement.y);
	if ((dynamicObject->GetCoordAndSize().x >= mapInfo.jCount * blockSize) || (dynamicObject->GetCoordAndSize().x + dynamicObject->GetCoordAndSize().width <= 0))
		dynamicObject->RespawnOrDispose();
}

void Environment::CollisionY(IDynamicObject* dynamicObject)
{
	UpdateCoords currInfo = GetCoordsForUpdate(dynamicObject->GetCoordAndSize());

	for (int i = currInfo.y1; i <= currInfo.y2; i++)
		for (int j = currInfo.x1; j <= currInfo.x2; j++)
		{
			if ((i < mapInfo.iCount && j < mapInfo.jCount) && (i >= 0 && j >= 0))
				switch (mapInfo.Map[i][j])
				{
				case 'B':
					simpleGround->SetCoord(j * blockSize, i * blockSize);
					simpleGround->InteractY(dynamicObject);
					break;
				case 'L':
					lava->InteractY(dynamicObject);
					break;
				default:
					break;
				}
		}
	if ((dynamicObject->GetCoordAndSize().y >= mapInfo.iCount * blockSize) || (dynamicObject->GetCoordAndSize().y + dynamicObject->GetCoordAndSize().height <= 0))
		dynamicObject->RespawnOrDispose();
}

void Environment::SetNonPlaybleObject(NonPlayble* object)
{
	if (currObjCount < maxObj)
	{
		nonPlaybleObjects[currObjCount] = object;
		nonPlaybleObjects[currObjCount]->SetG(g);
		currObjCount++;
	}
}

UpdateCoords Environment::GetCoordsForUpdate(CoordAndSize coordAndSize)
{
	UpdateCoords result;
	result.x1 = coordAndSize.x / blockSize;
	if ((coordAndSize.x + coordAndSize.width) % blockSize == 0)
		result.x2 = (coordAndSize.x + coordAndSize.width) / blockSize - 1;
	else
		result.x2 = (coordAndSize.x + coordAndSize.width) / blockSize;
	result.y1 = coordAndSize.y / blockSize;
	if ((coordAndSize.y + coordAndSize.height) % blockSize == 0)
		result.y2 = (coordAndSize.y + coordAndSize.height) / blockSize - 1;
	else
		result.y2 = (coordAndSize.y + coordAndSize.height) / blockSize;
	return result;
}

Environment::~Environment()
{
}
