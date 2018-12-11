#pragma once
#include "IStaticObject.h"

class Lava : public IStaticObject
{
public:
	Lava();
	void InteractX(IDynamicObject* dynamicObject) override;
	void InteractY(IDynamicObject* dynamicObject) override;
	void SetCoord(int x, int y) override;
	~Lava();
};

