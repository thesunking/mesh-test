#pragma once
#include "typedefs.h"

struct InputResponse
{
	InputResponse(keyBuffer keys, int mouseX, int mouseY);

	keyBuffer keys;
	int mouseX;
	int mouseY;
};