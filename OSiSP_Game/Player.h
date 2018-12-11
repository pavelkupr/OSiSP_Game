#pragma once
#include "IDynamicObject.h"
#include "Windows.h"
#include "basswv.h"

struct Controls
{
	bool right = false, left = false, up = false;
};

class Player : public IDynamicObject
{
public:
	Player(int, int, HBITMAP, int, int, int);
	void Move(Controls);
	void Move() override;
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
	void Interact(IDynamicObject*) override;
	void SetCoins(int) override;
	void SetKey(int) override;
	bool IsImmune() override;
	int GetLifeCount();
	int GetKeyCount();
	int GetCoinCount();
	void NextMap();
	bool GetIsNext();
	bool GetIsHit();
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

