#define _USE_MATH_DEFINES
#include "controlSys.h"
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

ControlSys::ControlSys(EntitySys& ref0, TerrainSys& ref1, CameraSys& ref2) : entSys(ref0), terSys(ref1), camSys(ref2) {}

void ControlSys::setPlayerID(ID id) { playerID = id; }

void ControlSys::update(InputResponse data, const float dt) {

	const float speed = 20.0f * dt;

	PhysicsCmp& playerCmp = entSys.get<PhysicsCmp>(playerID);

	//get 2d forward vector
	glm::vec3 fwd = camSys.getForwardVec();

	glm::vec3 fwd2 = glm::normalize(glm::vec3(fwd.x, 0.0f, fwd.z));

	//update player position
	if (data.keys[KEY_UP]) {
		playerCmp.p.x += fwd2.x * speed;
		playerCmp.p.z += fwd2.z * speed;
	}
	if (data.keys[KEY_DOWN]) {
		playerCmp.p.x -= fwd2.x * speed;
		playerCmp.p.z -= fwd2.z * speed;
	}
	if (data.keys[KEY_LEFT]) {
		playerCmp.p.x += fwd2.z * speed;
		playerCmp.p.z -= fwd2.x * speed;
	}
	if (data.keys[KEY_RIGHT]) {
		playerCmp.p.x -= fwd2.z * speed;
		playerCmp.p.z += fwd2.x * speed;
	}
	if (data.keys[KEY_FIRE]) {
		if ( playerCmp.p.y - terSys.getFloorHeight(playerCmp.p.x, playerCmp.p.z) < 0.2f ) {
			playerCmp.v.y += 5.0f;
		}
	}

	//printf("fwd: x %f y %f z %f\n", fwd.x, fwd.y, fwd.z);

	//update player direction
	const float x_sensitivity = 0.05f;
	const float y_sensitivity = 0.02f;
	float x_change = -data.mouseX * x_sensitivity;
	float y_change = -data.mouseY * y_sensitivity;

	/*
	//moving mouse in the x direction is a rotation about the local y-axis
	glm::quat q0(cosf(x_change), 0.0f, sinf(x_change), 0.0f);

	//moving mouse in the y direction is a rotation about the local x/z-axis
	//glm::vec3 xz_axis = glm::cross(fwd, glm::vec3(0.0f, 1.0f, 0.0f));
	//xz_axis = glm::normalize(xz_axis);
	//glm::quat q1(cosf(y_change), xz_axis*sinf(y_change));
	glm::quat q1(cosf(y_change), sinf(y_change), 0.0f, 0.0f);

	glm::quat q = q1*q0;

	//printf("qf: w %f x %f y %f z %f\n", q.w, q.x, q.y, q.z);

	playerCmp.o = q * playerCmp.o;

	printf("o: w %f x %f y %f z %f\n", playerCmp.o.w, playerCmp.o.x, playerCmp.o.y, playerCmp.o.z);
	*/

	camSys.yaw(x_change);
	camSys.pitch(y_change);

}

/* //finding quat that represents rotation from v0 to v1
	glm::quat q; 

	float dot = glm::dot(v0, v1);

	printf("dot: %f\n", dot);

	if (dot > 0.999999f) {
		//vectors are parallel, return unit quat
		q = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		glm::vec3 a = glm::cross(v0, v1);
		float v0_len = glm::length(v0);
		float v1_len = glm::length(v1);
		float w = sqrtf(v0_len*v0_len*v1_len*v1_len) + dot;
		q = glm::quat(w, a);
	}

	q = glm::normalize(q);
*/