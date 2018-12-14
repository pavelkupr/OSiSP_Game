#pragma once
#include <Windows.h>
#include "IDynamicObject.h"
#include "bass.h"

struct Controls
{
	bool right = false, left = false, up = false;
};

class Player : public IDynamicObject
{
public:
	Player(int, int, HBITMAP, int, int, int);
	void Move(Controls);
	void SetG(int) override;
	void SetOnGrnd() override;
	ObjDrawInfo GetDrawInfo() override;
	CoordAndSize GetCoordAndSize() override;
	PrevMovement GetPrevMovement() override;
	void SetCoord(int, int) override;
	void SetYSpeed(int) override;
	void RespawnOrDispose() override;
	void Kill() override;
	void Hit() override;
	bool IsLive() override;
	bool IsVisible() override;
	void SetCoins(int);
	void SetKey(int);
	bool IsImmune();
	int GetLifeCount();
	int GetKeyCount();
	int GetCoinCount();
	void NextMap();
	bool GetIsNext();
	bool GetIsHit();
	char name[10] = "Player";
	~Player();
private:
	int frameCount, g, _speed, _jump, ySpeed, lifeCount,coinCount, immuneCounter,keyCount;
	POINT resp;
	bool onGrnd, isLive, isVisible, isNext,isHited;
	HSTREAM jump, hit;
	float frameScale;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

