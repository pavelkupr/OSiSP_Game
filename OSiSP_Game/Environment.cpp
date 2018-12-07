#include "Environment.h"



Environment::Environment(Player* player,MapInfo mapInfo, int g)
{
	this->g = g;
	this->player = player;
	this->mapInfo = mapInfo;
	player->SetG(g);
	simpleGround = new SimpleGround();
	prevTime = GetTickCount();
}

void Environment::Cycle(Controls controls)
{
	player->Move(controls);
	CollisionX(player);
	CollisionY(player);
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
				default:
					break;
				}
		}
	if ((dynamicObject->GetCoordAndSize().y >= mapInfo.iCount * blockSize) || (dynamicObject->GetCoordAndSize().y + dynamicObject->GetCoordAndSize().height <= 0))
		dynamicObject->RespawnOrDispose();
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
