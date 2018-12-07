#pragma once
#include "Windows.h"

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
	bool isInvert;
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
	/*
	virtual void Move() = 0;
	
	virtual bool CollisionCheck(IDynamicObject*) = 0;
	virtual void Interact(IDynamicObject*) = 0;
	CoordAndSize coordAndSize;
	*/
};

