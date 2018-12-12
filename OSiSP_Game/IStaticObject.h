#pragma once
#include <Windows.h>
#include "IDynamicObject.h"

class IStaticObject
{
public:
	virtual void InteractX(IDynamicObject* dynamicObject) = 0;
	virtual void InteractY(IDynamicObject* dynamicObject) = 0;
	virtual void SetCoord(int x, int y) = 0;
};

