#pragma once
#include "IDynamicObject.h"
#include "Player.h"

class NonPlayble :
	public IDynamicObject
{
public:

	virtual void Interact(Player*) = 0;

	virtual void Move() = 0;//moveble

};

