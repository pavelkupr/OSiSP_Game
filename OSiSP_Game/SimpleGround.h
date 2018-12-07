#pragma once
#include "IStaticObject.h"

class SimpleGround : public IStaticObject
{
public:
	SimpleGround();
	void InteractX(IDynamicObject* dynamicObject) override;
	void InteractY(IDynamicObject* dynamicObject) override;
	void SetCoord(int x, int y) override;
	~SimpleGround();
private:
	int x, y;
	const int size = 32;
};

