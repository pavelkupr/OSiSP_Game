#include "Lava.h"

Lava::Lava()
{
}

void Lava::InteractX(IDynamicObject* dynamicObject)
{
	if (!dynamicObject->IsLive())
		return;
	dynamicObject->Kill();
}

void Lava::InteractY(IDynamicObject* dynamicObject)
{
	if (!dynamicObject->IsLive())
		return;
	dynamicObject->Kill();
}

void Lava::SetCoord(int x, int y)
{

}

Lava::~Lava()
{
}
