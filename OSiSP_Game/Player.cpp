#include "Player.h"

Player::Player(int x, int y, HBITMAP img, int frames,int jump,int speed)
{
	BITMAP bm;
	GetObject(img, sizeof(BITMAP), (LPSTR)&bm);
	resp.x = x;
	resp.y = y;
	coordAndSize.x = x;
	coordAndSize.y = y;
	coordAndSize.width = bm.bmWidth / frames;
	coordAndSize.height = bm.bmHeight;

	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isInvert = false;
	onGrnd = false;
	_jump = jump;
	_speed = speed;
	ySpeed = 0;
	frameScale = 1;
	frameCount = frames;
}

void Player::Move(Controls controls)
{
	prevMovement.x = 0;
	prevMovement.y = 0;
	bool move = false;
	if (controls.right)
	{
		coordAndSize.x += _speed;
		prevMovement.x = _speed;
		drawInfo.isInvert = true;
		move = true;
	}
	if (controls.left)
	{
		coordAndSize.x -= _speed;
		prevMovement.x = -_speed;
		drawInfo.isInvert = false;
		if (move)
			move = false;
		else
			move = true;
	}
	
	if (!onGrnd)
	{
		drawInfo.currFrame = 4;
	}
	else if(move)
	{
		frameScale += 0.2;
		if (frameScale >= frameCount)
			frameScale = 1;
		drawInfo.currFrame = (int)frameScale;
	}
	else
	{
		drawInfo.currFrame = 0;
	}

	if (onGrnd)
	{
		if (controls.up)
		{
			ySpeed = _jump;
			coordAndSize.y -= ySpeed;
			prevMovement.y = -ySpeed;
			onGrnd = false;
		}
		else
		{
			ySpeed -= g;
			coordAndSize.y -= ySpeed;
			prevMovement.y = -ySpeed;
			onGrnd = false;
		}
	}
	else
	{
		ySpeed -= g;
		coordAndSize.y -= ySpeed;
		prevMovement.y = -ySpeed;
	}


}

void Player::RespawnOrDispose()
{
	coordAndSize.x = resp.x;
	coordAndSize.y = resp.y;
}

void Player::SetYSpeed(int ySpeed)
{
	this->ySpeed = ySpeed;
}

PrevMovement Player::GetPrevMovement()
{
	return prevMovement;
}

ObjDrawInfo Player::GetDrawInfo()
{
	return drawInfo;
}

CoordAndSize Player::GetCoordAndSize()
{
	return coordAndSize;
}

void Player::SetCoord(int x, int y)
{
	coordAndSize.x = x;
	coordAndSize.y = y;
}

void Player::SetG(int value)
{
	g = value;
}

void Player::SetOnGrnd()
{
	onGrnd = true;
}

Player::~Player()
{
}
