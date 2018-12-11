#include "Key.h"



Key::Key(int x, int y, HBITMAP img, int frames, int mode)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	coordAndSize.x = x;
	coordAndSize.y = y;
	coordAndSize.width = bm.bmWidth / frames;
	coordAndSize.height = bm.bmHeight;

	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = false;
	onGrnd = false;
	isLive = true;
	isVisible = true;
	this->mode = mode;
	ySpeed = 0;
	frameScale = rand() % frames;
	lifeCount = 1;
	frameCount = frames;
}

void Key::Move()
{
	prevMovement.x = 0;
	prevMovement.y = 0;
	bool move = false;
	if (lifeCount <= 0 && isLive)
	{
		isLive = false;
		onGrnd = true;
	}

	frameScale += 0.5;
	if (frameScale >= frameCount)
		frameScale = 0;
	drawInfo.currFrame = (int)frameScale;

	if (mode == 1)
	{
		if (onGrnd)
		{

			ySpeed -= g;
			coordAndSize.y -= ySpeed;
			prevMovement.y = -ySpeed;
			onGrnd = false;

		}
		else
		{
			ySpeed -= g;
			coordAndSize.y -= ySpeed;
			prevMovement.y = -ySpeed;
		}
	}

}

void Key::Interact(IDynamicObject* object)
{
	object->SetKey(1);
	isVisible = false;
}

bool Key::IsLive()
{
	return isLive;
}

void Key::RespawnOrDispose()
{
	if (isVisible)
		isVisible = false;
	else
		isVisible = true;
}

bool Key::IsVisible()
{
	return isVisible;
}

void Key::SetYSpeed(int ySpeed)
{
	this->ySpeed = ySpeed;
}

void Key::Kill()
{
	lifeCount = 0;
}

void Key::Hit()
{
	lifeCount--;
}

PrevMovement Key::GetPrevMovement()
{
	return prevMovement;
}

ObjDrawInfo Key::GetDrawInfo()
{
	return drawInfo;
}

CoordAndSize Key::GetCoordAndSize()
{
	return coordAndSize;
}

void Key::SetCoord(int x, int y)
{
	coordAndSize.x = x;
	coordAndSize.y = y;
}

void Key::SetCoins(int count)
{
}

void Key::SetG(int value)
{
	g = value;
}

void Key::SetOnGrnd()
{
	onGrnd = true;
}
bool Key::IsImmune() { return false; }
void Key::SetKey(int) {}
Key::~Key()
{
}
