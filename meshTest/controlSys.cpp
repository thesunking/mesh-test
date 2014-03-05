#define _USE_MATH_DEFINES
#include "controlSys.h"
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

ControlSys::ControlSys(EntitySys& ref0, TerrainSys& ref1, CameraSys& ref2) : entSys(ref0), terSys(ref1), camSys(ref2)
{
}

void ControlSys::setPlayerID(ID id)
{
	playerID = id;
}

void ControlSys::update(InputResponse data, const float dt)
{
	const float speed = 20.0f * dt;

	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);

	//get 2d forward vector
	glm::vec3 fwd = camSys.getForwardVec();

	glm::vec3 fwd2 = glm::normalize(glm::vec3(fwd.x, 0.0f, fwd.z));

	//update player position
	if (data.keys[KEY_UP])
	{
		playerCmp.p.x += fwd2.x * speed;
		playerCmp.p.z += fwd2.z * speed;
	}
	if (data.keys[KEY_DOWN])
	{
		playerCmp.p.x -= fwd2.x * speed;
		playerCmp.p.z -= fwd2.z * speed;
	}
	if (data.keys[KEY_LEFT])
	{
		playerCmp.p.x += fwd2.z * speed;
		playerCmp.p.z -= fwd2.x * speed;
	}
	if (data.keys[KEY_RIGHT])
	{
		playerCmp.p.x -= fwd2.z * speed;
		playerCmp.p.z += fwd2.x * speed;
	}
	if (data.keys[KEY_FIRE])
	{
		if ( playerCmp.p.y - terSys.getFloorHeight(playerCmp.p.x, playerCmp.p.z) < 0.2f )
		{
			playerCmp.v.y += 5.0f;
		}
	}

	//update player direction
	const float x_sensitivity = 0.05f;
	const float y_sensitivity = 0.02f;
	float x_change = -data.mouseX * x_sensitivity;
	float y_change = -data.mouseY * y_sensitivity;

	camSys.yaw(x_change);
	camSys.pitch(y_change);
}