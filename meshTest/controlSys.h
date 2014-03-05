#pragma once
#include "entitySys.h"
#include "terrainSys.h"
#include "inputResponse.h"
#include "cameraSys.h"

class ControlSys
{
private:
	EntitySys& entSys;
	TerrainSys& terSys;
	CameraSys& camSys;
	ID playerID;

public:
	ControlSys(EntitySys& ref0, TerrainSys& ref1, CameraSys& ref2);

	void setPlayerID(ID id);
	void update(InputResponse data, const float dt);
};