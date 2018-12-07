#pragma once
#include "IDynamicObject.h"
#include "Windows.h"

class IStaticObject
{
public:
	virtual void InteractX(IDynamicObject* dynamicObject) = 0;
	virtual void InteractY(IDynamicObject* dynamicObject) = 0;
	virtual void SetCoord(int x, int y) = 0;
};

