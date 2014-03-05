#pragma once
#include "terrainSys.h"
#include "controlSys.h"
#include "cameraSys.h"
#include "physicsSys.h"
#include "renderSys.h"
#include "entitySys.h"
#include "typedefs.h"
#include "inputResponse.h"

class WorldSys
{
private:
	EntitySys entity;
	TerrainSys terrain;
	ControlSys control;
	CameraSys camera;
	PhysicsSys physics;

	ID testBlockID;

	float sunAngle;

public:
	WorldSys(RenderSys &render);

	gameState update(InputResponse data, sf::Window &window, float t, const float dt);

	void draw(RenderSys &render);

	void drawAll(RenderSys &render);
};