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
	this->jump = BASS_StreamCreateFile(FALSE, TEXT("jump.wav"), 0, 0, 0);
	hit = BASS_StreamCreateFile(FALSE, TEXT("hit.wav"), 0, 0, 0);
	drawInfo.bitmap = img;
	drawInfo.currFrame = 0;
	drawInfo.isDraw = true;
	drawInfo.isInvert = true;
	onGrnd = false;
	isNext = false;
	isLive = true;
	isHited = false;
	keyCount = 0;
	immuneCounter = 0;
	isVisible = true;
	_jump = jump;
	_speed = speed;
	coinCount = 0;
	ySpeed = 0;
	frameScale = 1;
	lifeCount = 3;
	frameCount = frames;
}

void Player::Move(Controls controls)
{
	if (immuneCounter > 0)
	{
		immuneCounter--;
	}
	if ((immuneCounter / 3) % 2 == 1)
	{
		drawInfo.isDraw = false;
	}
	else
	{
		drawInfo.isDraw = true;
	}
	prevMovement.x = 0;
	prevMovement.y = 0;
	bool move = false;
	if (lifeCount <= 0 && isLive)
	{
		isLive = false;
		onGrnd = true;
		ySpeed = _jump;
	}
	else if (isLive)
	{
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
	}
	if (move)
	{
		frameScale += 0.25;
		if (frameScale >= frameCount)
			frameScale = 1;
		drawInfo.currFrame = (int)frameScale;
	}
	else
	{
		if (!onGrnd)
		{
			drawInfo.currFrame = 4;
		}
		else
			drawInfo.currFrame = 0;
	}

	if (onGrnd)
	{
		if (controls.up)
		{
			BASS_ChannelPlay(jump, TRUE);
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

int Player::GetCoinCount()
{
	return coinCount;
}

void Player::NextMap()
{
	isNext=true;
}

bool Player::GetIsHit()
{
	if (isHited == true)
	{
		isHited = false;
		return true;
	}
	return false;
}

bool Player::GetIsNext()
{
	if (isNext == true)
	{
		isNext = false;
		return true;
	}
	return false;
}

int Player::GetLifeCount()
{
	return lifeCount;
}

bool Player::IsLive()
{
	return isLive;
}

void Player::RespawnOrDispose()
{
	isLive = true;
	lifeCount = 3;
	coinCount = 0;
	keyCount = 0;
	ySpeed = 0;
	coordAndSize.x = resp.x;
	coordAndSize.y = resp.y;
}

bool Player::IsVisible()
{
	return isVisible;
}

bool Player::IsImmune()
{
	if(immuneCounter == 0)
		return false;
	return true;
}

void Player::SetYSpeed(int ySpeed)
{
	this->ySpeed = ySpeed;
}

void Player::Kill()
{
	lifeCount = 0;
	BASS_ChannelPlay(hit, TRUE);
}

void Player::Hit()
{
	lifeCount--;
	if (lifeCount > 0)
	{
		isHited = true;
		ySpeed = _jump;
		onGrnd = false;
		immuneCounter = 60;
	}
	BASS_ChannelPlay(hit, TRUE);
	//PlaySound(L"hit.wav", NULL, SND_FILENAME | SND_ASYNC);
	
}

void Player::SetKey(int count)
{
	keyCount += count;
}

int Player::GetKeyCount()
{
	return keyCount;
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

void Player::SetCoins(int count)
{
	coinCount += count;
}

void Player::Interact(IDynamicObject*)
{

}
void Player::Move()
{}
Player::~Player()
{
}
