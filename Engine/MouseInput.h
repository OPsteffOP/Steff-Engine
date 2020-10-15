#pragma once
#include "Mouse.h"
#include "InputType.h"

struct MouseInput
{
	Mouse mouse;
	Type type;
	XMFLOAT2 position;
};