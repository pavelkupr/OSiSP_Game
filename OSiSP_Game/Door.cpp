#include "Door.h"

Door::Door(int x, int y, HBITMAP img,Player* player)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	coordAndSize.x = x;
	coordAndSize.y = y;
	prevMovement.x = 0;
	prevMovement.y = 0;
	coordAndSize.width = bm.bmWidth / 2;
	coordAndSize.height = bm.bmHeight;
	this->player = player;
	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = false;
	isLive = true;
	isVisible = true;
}

void Door::Move()
{
	if(player->GetKeyCount()>0)
		drawInfo.currFrame = 1;
	else
		drawInfo.currFrame = 0;

}

void Door::Interact(Player* object)
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

void Door::SetYSpeed(int ySpeed) {}
void Door::SetG(int value){}
void Door::SetOnGrnd(){}
void Door::Kill() {}
void Door::Hit() {}
Door::~Door()
{
}
