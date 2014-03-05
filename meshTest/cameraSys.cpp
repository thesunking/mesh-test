#define _USE_MATH_DEFINES

#include "cameraSys.h"
#include "typedefs.h"
#include "SFML\OpenGL.hpp"
#include <cmath>

CameraSys::CameraSys(EntitySys& ref) : entSys(ref)
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	left = glm::vec3(0.0f, 0.0f, -1.0f);
	fwd = glm::vec3(1.0f, 0.0f, 0.0f);
}

void CameraSys::setPlayerID(ID id)
{
	playerID = id;
}


glm::vec3 CameraSys::getForwardVec()
{
	return fwd;
}

glm::vec3 CameraSys::getLeftVec()
{
	return left;
}

glm::vec3 CameraSys::getUpVec()
{
	return up;
}

void CameraSys::roll(float angle)
{
	glm::quat q = glm::angleAxis(angle, fwd);

	//up	 = q * up * glm::conjugate(q);
	left = q * left * glm::conjugate(q);

	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);
	playerCmp.o = q * playerCmp.o;
}

void CameraSys::pitch(float angle)
{
	glm::quat q = glm::angleAxis(angle, left);

	//up	= q * up * glm::conjugate(q);
	fwd = q * fwd * glm::conjugate(q);

	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);
	playerCmp.o = q * playerCmp.o;
}

void CameraSys::yaw(float angle)
{
	glm::quat q = glm::angleAxis(angle, up);

	left = q * left * glm::conjugate(q);
	fwd = q * fwd * glm::conjugate(q);

	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);
	playerCmp.o = q * playerCmp.o;
}

glm::mat4 CameraSys::updateView()
{
	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);
	glm::vec3 pos = playerCmp.p;
	pos.y += 5.0f;

	glm::vec3 center = pos + fwd;

	return glm::lookAt(pos, center, up);
}