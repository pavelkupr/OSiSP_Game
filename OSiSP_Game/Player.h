#pragma once
#include "IDynamicObject.h"
#include "Windows.h"

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
	~Player();
private:
	int frameCount, g, _speed, _jump, ySpeed;
	POINT resp;
	bool onGrnd;
	float frameScale;
	PrevMovement prevMovement;
	ObjDrawInfo drawInfo;
	CoordAndSize coordAndSize;
};

