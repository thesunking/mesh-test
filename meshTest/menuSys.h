#pragma once
#include "inputSys.h"
#include "renderSys.h"
#include "typedefs.h"

class MenuSys
{
private:

public:


	gameState update(InputResponse data);

	void draw(RenderSys &render);
};