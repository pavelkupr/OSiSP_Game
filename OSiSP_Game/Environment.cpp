#include "Environment.h"



Environment::Environment(Drawer* drawer, Player* player, const char* map, int g)
{
	this->g = g;
	this->player = player;
	this->map = new Map();
	this->map->LoadMap(map);
	this->mapInfo = this->map->GetMapInfo();
	this->drawer = drawer;
	coinSound = BASS_StreamCreateFile(FALSE, TEXT("coin.wav"), 0, 0, 0);
	firstMap = mapInfo;
	drawer->SetPlayer(player);
	player->SetG(g);
	prevPlayerState = true;
	dynamicobjects = (IDynamicObject**)malloc(sizeof(IDynamicObject**) * maxObj);
	for (int i = 0; i < maxObj; i++)
	{
		dynamicobjects[i] = 0;
	}
	currObjCount = 0;
	simpleGround = new SimpleGround();
	lava = new Lava();
	drawer->SetDrawInfo(mapInfo);
	drawer->SetDynamicObjects(dynamicobjects, &currObjCount);
	ObjectCreation();
}

void Environment::NextMap()
{
	if (mapInfo.pathToNext != firstMap.pathToNext)
		map->DeleteMapInfo(mapInfo);
	player->SetCoord(64, 64);
	map->LoadMap(mapInfo.pathToNext);
	mapInfo = map->GetMapInfo();
	SetMap(mapInfo);
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
				SetDynamicObject(new Coin(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount, mapInfo.objsInfo[i].mode, coinSound));
				break;
			case'E':
				SetDynamicObject(new Enemy1(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount,4, mapInfo.objsInfo[i].mode*blockSize));
				break;
			case'K':
				SetDynamicObject(new Key(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize, (HBITMAP)curr, mapInfo.objsInfo[i].frameCount, mapInfo.objsInfo[i].mode));
				break;
			case'D':
				SetDynamicObject(new Door(mapInfo.objsInfo[i].coords[j].x*blockSize, mapInfo.objsInfo[i].coords[j].y*blockSize+4, (HBITMAP)curr, player));
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
		if (dynamicobjects[i] != 0)
		{
			if (dynamicobjects[i]->IsVisible())
			{
				dynamicobjects[i]->Move();
				CollisionX(dynamicobjects[i]);
				CollisionY(dynamicobjects[i]);
			}

			if(!dynamicobjects[i]->IsVisible())
			{
				delete dynamicobjects[i];
				dynamicobjects[i] = 0;
				for (int j = i + 1; j < currObjCount; j++)
				{
					dynamicobjects[j - 1] = dynamicobjects[j];
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
				if (dynamicobjects[i]->IsVisible())
				{
					if (player->GetCoordAndSize().x + player->GetCoordAndSize().width > dynamicobjects[i]->GetCoordAndSize().x &&
						player->GetCoordAndSize().x < dynamicobjects[i]->GetCoordAndSize().x + dynamicobjects[i]->GetCoordAndSize().width)
						if (player->GetCoordAndSize().y + player->GetCoordAndSize().height > dynamicobjects[i]->GetCoordAndSize().y &&
							player->GetCoordAndSize().y < dynamicobjects[i]->GetCoordAndSize().y + dynamicobjects[i]->GetCoordAndSize().height)
							dynamicobjects[i]->Interact(player);
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
				SetDynamicObject(new Coin(player->GetCoordAndSize().x,
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

void Environment::SetDynamicObject(IDynamicObject* object)
{
	if (currObjCount < maxObj)
	{
		dynamicobjects[currObjCount] = object;
		dynamicobjects[currObjCount]->SetG(g);
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
