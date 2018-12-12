#pragma once
#include <Windows.h>

struct PrevMovement
{
	int x;
	int y;
};

struct CoordAndSize
{
	int x;
	int y;
	int width;
	int height;
};

struct ObjDrawInfo
{
	HBITMAP bitmap;
	int currFrame;
	bool isInvert, isDraw;
};

class IDynamicObject
{
public:
	virtual void SetG(int) = 0;
	virtual void SetOnGrnd() = 0;
	virtual ObjDrawInfo GetDrawInfo() = 0;
	virtual CoordAndSize GetCoordAndSize() = 0;
	virtual PrevMovement GetPrevMovement() = 0;
	virtual void SetCoord(int, int) = 0;
	virtual void SetYSpeed(int) = 0;
	virtual void RespawnOrDispose() = 0;
	virtual void Kill() = 0;
	virtual void Hit() = 0;
	virtual bool IsLive() = 0;
	virtual bool IsVisible() = 0;
};

