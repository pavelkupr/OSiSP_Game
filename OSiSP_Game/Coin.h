#pragma once
#include "NonPlayble.h"
#include "Player.h"
#include "bass.h"

class Coin : public NonPlayble
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
	void Interact(Player*) override;
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

