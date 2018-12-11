#pragma once
#include "IDynamicObject.h"
#include "Player.h"

class Door :
	public IDynamicObject
{
public:
	Door(int, int, HBITMAP,Player*);
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
	~Door();
private:
	int frameCount, g, ySpeed, lifeCount, mode;
	bool onGrnd, isLive, isVisible;
	float frameScale;
	Player* player;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

