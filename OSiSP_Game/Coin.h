#pragma once
#include "IDynamicObject.h"
#include "basswv.h"

class Coin : public IDynamicObject
{
public:
	Coin(int, int, HBITMAP, int,int,HSTREAM);
	void SetG(int) override;
	void SetOnGrnd() override;
	void Move() override;
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
	~Coin();
private:
	int frameCount, g, ySpeed, xSpeed,lifeCount, mode, immuneCounter;
	bool onGrnd, isLive, isVisible;
	float frameScale;
	HSTREAM coin;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

