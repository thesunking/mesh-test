#pragma once
#include "lookupID.h"
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class PhysicsCmp
{
public:
	ID id;

	float mass;
	glm::mat3 inertia;
	glm::mat3 invInertia;

	glm::vec3 p; //position
	glm::vec3 v; //linear velocity
	glm::vec3 w; //angular velocity
	glm::quat o; //orientation

	PhysicsCmp();
	PhysicsCmp(glm::vec3 position);

	void updateInertiaMatrix( float x, float y, float z );
};