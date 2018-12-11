#pragma once
#include "IDynamicObject.h"
class Key :
	public IDynamicObject
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
	void Interact(IDynamicObject*) override;
	void SetCoins(int) override;
	void SetKey(int) override;
	bool IsImmune() override;
	~Key();
private:
	int frameCount, g, ySpeed, lifeCount, mode;
	bool onGrnd, isLive, isVisible;
	float frameScale;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

