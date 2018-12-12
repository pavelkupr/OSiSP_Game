#pragma once
#include "NonPlayble.h"
#include "Player.h"

class Enemy1 :
	public NonPlayble
{
public:
	Enemy1(int, int, HBITMAP, int, int, int);
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
	void Interact(Player*) override;
	~Enemy1();
private:
	void MoveType1();
	void MoveType2();
	int frameCount, g, _speed, dest, ySpeed, lifeCount, coinCount, start, end,mode;
	POINT resp;
	bool onGrnd, isLive, isVisible;
	float frameScale;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

