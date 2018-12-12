#include "Enemy1.h"

Enemy1::Enemy1(int x, int y, HBITMAP img, int frames, int speed, int Final)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	resp.x = x;
	resp.y = y;
	coordAndSize.x = x;
	coordAndSize.y = y;
	coordAndSize.width = bm.bmWidth / frames;
	coordAndSize.height = bm.bmHeight;
	prevMovement.x = 0;
	prevMovement.y = 0;
	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = false;
	onGrnd = false;
	isLive = true;
	isVisible = true;
	_speed = speed;
	coinCount = 0;
	ySpeed = 0;
	frameScale = 1;
	lifeCount = 3;
	frameCount = frames;
	if (Final >= 0)
	{
		mode = 0;
		end = Final;
		start = x;
		dest = Final;
	}
	else
	{
		mode = 1;
		end = -Final;
		start = y;
		dest = -Final;
	}
}

void Enemy1::Move()
{
	if (mode == 0)
	{
		MoveType1();
	}
	else if (mode == 1)
	{
		MoveType2();
	
	}
}

void Enemy1::MoveType1() 
{
	if (prevMovement.x < 0 && coordAndSize.x <= dest)
	{
		if (dest == start)
			dest = end;
		else
			dest = start;
	}
	else if (prevMovement.x > 0 && coordAndSize.x >= dest)
	{
		if (dest == start)
			dest = end;
		else
			dest = start;
	}

	if (lifeCount <= 0 && isLive)
	{
		isLive = false;
	}
	else if (isLive)
	{
		if (coordAndSize.x <= dest)
		{
			coordAndSize.x += _speed;
			prevMovement.x = _speed;
			drawInfo.isInvert = true;
		}
		if (coordAndSize.x >= dest)
		{
			coordAndSize.x -= _speed;
			prevMovement.x = -_speed;
			drawInfo.isInvert = false;
		}
	}
	if (!onGrnd)
	{
		drawInfo.currFrame = 4;
	}
	else
	{
		frameScale += 0.3;
		if (frameScale >= frameCount)
			frameScale = 1;
		drawInfo.currFrame = (int)frameScale;
	}

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

void Enemy1::MoveType2() 
{
	if (prevMovement.y < 0 && coordAndSize.y <= dest)
	{
		if (dest == start)
			dest = end;
		else
			dest = start;
	}
	else if (prevMovement.y > 0 && coordAndSize.y >= dest)
	{
		if (dest == start)
			dest = end;
		else
			dest = start;
	}

	if (lifeCount <= 0 && isLive)
	{
		isLive = false;
	}
	else if (isLive)
	{
		frameScale += 0.3;
		if (frameScale >= frameCount)
			frameScale = 0;
		drawInfo.currFrame = (int)frameScale;
		if (coordAndSize.y <= dest)
		{
			coordAndSize.y += _speed;
			prevMovement.y = _speed;
			drawInfo.isInvert = true;
		}
		if (coordAndSize.y >= dest)
		{
			coordAndSize.y -= _speed;
			prevMovement.y = -_speed;
			drawInfo.isInvert = false;
		}
	}
	else
	{
		coordAndSize.y += _speed;
		prevMovement.y = _speed;
	}
}

void Enemy1::Interact(Player* obj)
{
	if(!obj->IsImmune())
		obj->Hit();
}

bool Enemy1::IsLive()
{
	return isLive;
}

void Enemy1::RespawnOrDispose()
{
	isLive = true;
	lifeCount = 3;
	coinCount = 0;
	coordAndSize.x = resp.x;
	coordAndSize.y = resp.y;
}

bool Enemy1::IsVisible()
{
	return isVisible;
}

void Enemy1::SetYSpeed(int ySpeed)
{
	this->ySpeed = ySpeed;
}

void Enemy1::Kill()
{
	lifeCount = 0;
}

void Enemy1::Hit()
{
	lifeCount--;
}

PrevMovement Enemy1::GetPrevMovement()
{
	return prevMovement;
}

ObjDrawInfo Enemy1::GetDrawInfo()
{
	return drawInfo;
}

CoordAndSize Enemy1::GetCoordAndSize()
{
	return coordAndSize;
}

void Enemy1::SetCoord(int x, int y)
{
	coordAndSize.x = x;
	coordAndSize.y = y;
}

void Enemy1::SetG(int value)
{
	g = value;
}

void Enemy1::SetOnGrnd()
{
	onGrnd = true;
}

Enemy1::~Enemy1()
{
}
