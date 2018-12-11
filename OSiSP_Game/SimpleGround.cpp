#include "SimpleGround.h"



SimpleGround::SimpleGround()
{
	
}

void SimpleGround::InteractX(IDynamicObject* dynamicObject)
{
	if (!dynamicObject->IsLive())
		return;
	PrevMovement prevMovement = dynamicObject->GetPrevMovement();
	CoordAndSize coordAndSize = dynamicObject->GetCoordAndSize();
	int newX = coordAndSize.x, newY = coordAndSize.y;
	if (prevMovement.x > 0)
		newX = x - coordAndSize.width;
	else if (prevMovement.x < 0)
		newX = x + size;
	dynamicObject->SetCoord(newX,newY);
}

void SimpleGround::InteractY(IDynamicObject* dynamicObject)
{
	if (!dynamicObject->IsLive())
		return;
	PrevMovement prevMovement = dynamicObject->GetPrevMovement();
	CoordAndSize coordAndSize = dynamicObject->GetCoordAndSize();
	int newX = coordAndSize.x, newY = coordAndSize.y;
	if (prevMovement.y > 0)
	{
		newY = y - coordAndSize.height;
		dynamicObject->SetYSpeed(0);
		dynamicObject->SetOnGrnd();
	}
	else if (prevMovement.y < 0)
	{
		newY = y + size;
		dynamicObject->SetYSpeed(0);
	}

	dynamicObject->SetCoord(newX, newY);
}

void SimpleGround::SetCoord(int x, int y)
{
	this->x = x;
	this->y = y;
}

SimpleGround::~SimpleGround()
{
}
