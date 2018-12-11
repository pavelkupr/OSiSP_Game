#include "Coin.h"


Coin::Coin(int x, int y, HBITMAP img,int frames, int mode, HSTREAM sound)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	coordAndSize.x = x;
	coordAndSize.y = y;
	coordAndSize.width = bm.bmWidth / frames;
	coordAndSize.height = bm.bmHeight;
	coin = sound;
	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = false;
	onGrnd = false;
	isLive = true;
	isVisible = true;
	this->mode = mode;
	if (mode == 1)
	{
		immuneCounter = 120;
		if (rand() % 2 == 1)
			ySpeed = 10 + rand() % 10;
		else
			ySpeed = -(10 + rand() % 10);
		if(rand() % 2 == 1)
			xSpeed = 10 + rand() % 10;
		else
			xSpeed = -(10 + rand() % 10);
	}
	else
	{
		immuneCounter = 0;
		ySpeed = 0;
		xSpeed = 0;
	}
	frameScale = rand() % frames;
	lifeCount = 1;
	frameCount = frames;
}

void Coin::Move()
{
	if(mode == 1)
	{	
		immuneCounter--;
		if (immuneCounter > 0)
		{
			if ((immuneCounter / 4) % 2 == 1)
			{
				drawInfo.isDraw = false;
			}
			else
			{
				drawInfo.isDraw = true;
			}
		}
		else
		{
			isVisible = false;
		}
	}
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
		xSpeed = xSpeed / 1.1;
		coordAndSize.x -= xSpeed;
		prevMovement.x = -xSpeed;
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

void Coin::Interact(IDynamicObject* object)
{
	if (!object->IsImmune())
	{
		BASS_ChannelPlay(coin, TRUE);
		//PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC);
		object->SetCoins(1);
		isVisible = false;
	}
}

bool Coin::IsLive()
{
	return isLive;
}

void Coin::RespawnOrDispose()
{
	if(isVisible)
		isVisible = false;
}

bool Coin::IsVisible()
{
	return isVisible;
}

void Coin::SetYSpeed(int ySpeed)
{	
	if(ySpeed == 0)
		this->ySpeed = -this->ySpeed/1.2;
	else
		this->ySpeed = ySpeed;
}

void Coin::Kill()
{
	lifeCount = 0;
}

void Coin::Hit()
{
	lifeCount--;
}

PrevMovement Coin::GetPrevMovement()
{
	return prevMovement;
}

ObjDrawInfo Coin::GetDrawInfo()
{
	return drawInfo;
}

CoordAndSize Coin::GetCoordAndSize()
{
	return coordAndSize;
}

void Coin::SetCoord(int x, int y)
{
	if (coordAndSize.x != x)
		xSpeed = -xSpeed;
	coordAndSize.x = x;
	coordAndSize.y = y;
}

void Coin::SetCoins(int count)
{
}

void Coin::SetG(int value)
{
	g = value;
}

void Coin::SetOnGrnd()
{
	onGrnd = true;
}
bool Coin::IsImmune() { return false; }
void Coin::SetKey(int) {}
Coin::~Coin()
{
}
