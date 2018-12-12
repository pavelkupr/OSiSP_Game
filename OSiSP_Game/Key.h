#pragma once
#include "NonPlayble.h"
#include "Player.h"

class Key :
	public NonPlayble
{
public:
	Key(int, int, HBITMAP, int, int);
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
	~Key();
private:
	int frameCount, g, ySpeed, lifeCount, mode;
	bool onGrnd, isLive, isVisible;
	float frameScale;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

