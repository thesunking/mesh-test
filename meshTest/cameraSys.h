#pragma once
#include "entitySys.h"
#include "inputResponse.h"

//reference: https://github.com/dustin-biser/Rigid3D/blob/master/src/utils/GlUtils/Camera.cpp

class CameraSys
{
private:
	EntitySys& entSys;
	ID playerID;

	glm::vec3 up;
	glm::vec3 left;
	glm::vec3 fwd;

public:
	CameraSys(EntitySys& ref);

	void setPlayerID(ID id);

	glm::vec3 getForwardVec();
	glm::vec3 getLeftVec();
	glm::vec3 getUpVec();

	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);

	glm::mat4 updateView();
};