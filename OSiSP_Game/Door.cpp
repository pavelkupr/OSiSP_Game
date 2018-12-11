#include "Door.h"



Door::Door(int x, int y, HBITMAP img,Player* player)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	coordAndSize.x = x;
	coordAndSize.y = y;
	coordAndSize.width = bm.bmWidth / 2;
	coordAndSize.height = bm.bmHeight;
	this->player = player;
	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = false;
	isLive = true;
	isVisible = true;
	lifeCount = 1;
}

void Door::Move()
{
	if(player->GetKeyCount()>0)
		drawInfo.currFrame = 1;
	else
		drawInfo.currFrame = 0;

}

void Door::Interact(IDynamicObject* object)
{
	if (player->GetKeyCount() > 0)
	{
		player->SetKey(-1);
		player->NextMap();
	}
}

bool Door::IsLive()
{
	return isLive;
}

void Door::RespawnOrDispose()
{
	if (isVisible)
		isVisible = false;
	else
		isVisible = true;
}

bool Door::IsVisible()
{
	return isVisible;
}

void Door::SetYSpeed(int ySpeed)
{
	this->ySpeed = ySpeed;
}

void Door::Kill()
{
	lifeCount = 0;
}

void Door::Hit()
{
	lifeCount--;
}

PrevMovement Door::GetPrevMovement()
{
	return prevMovement;
}

ObjDrawInfo Door::GetDrawInfo()
{
	return drawInfo;
}

CoordAndSize Door::GetCoordAndSize()
{
	return coordAndSize;
}

void Door::SetCoord(int x, int y)
{
	coordAndSize.x = x;
	coordAndSize.y = y;
}

void Door::SetCoins(int count)
{
}

void Door::SetG(int value)
{
	g = value;
}

void Door::SetOnGrnd()
{
	onGrnd = true;
}
bool Door::IsImmune() { return false; }
void Door::SetKey(int) {}
Door::~Door()
{
}
