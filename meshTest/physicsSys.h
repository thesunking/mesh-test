#pragma once
#include "entitySys.h"
#include "terrainSys.h"
#include "contactSys.h"
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>


struct State {

	glm::vec3 p;
	glm::vec3 v;
	glm::vec3 w;

	State();
	State(PhysicsCmp &cmp);

};

struct Derivative {

    glm::vec3 dx;
    glm::vec3 dv;
	glm::vec3 dw;

	Derivative();

};

struct Contact {

	glm::vec3 p;	//position of contact
	glm::vec3 r;	//(p-center of object)
	glm::vec3 n;	//normal of contact
	glm::vec3 dp;	//v + (w x r)
	float pen;	//penetration along normal

	Contact(glm::vec3 position, glm::vec3 radius, glm::vec3 normal, glm::vec3 rel_velocity, float penetration);

};


class PhysicsSys {

private:
	EntitySys&  entSys;
	TerrainSys& terSys;


	const float gravity;
	const float drag;


	Derivative evaluate(State& initial, float t, float dt, Derivative &deriv);
	glm::vec3  acceleration(State& state, float t);
	State	   integrate(State& state, float t, float dt);


	void updateComponentState(PhysicsCmp& cmp, State& state);
	void updateComponentState(PhysicsCmp& cmp, RenderCmp& rndCmp, State& state, float dt);

	glm::quat getQuaternionFromAngularVelocity(glm::vec3 ang_vel, float dt);
	glm::mat3 getRotationMatrixFromQuaternion(glm::quat oreintation);

	void clampAboveGround(PhysicsCmp& cmp);
	float ground(glm::vec3 vertex);
	float ground(glm::vec3 vertex, glm::vec3 normal);


	//resolve_object helper functions
	std::vector<Contact> findGroundContacts(PhysicsCmp& phyCmp, RenderCmp& rndCmp, float t, float dt);

	int getContactType(Contact& contact);
	float findTimeOfCollision(Contact c, float dt);

	std::vector<float> calcRestingForces(PhysicsCmp& phyCmp, std::vector<Contact> contacts);
	std::vector<float> calcCollidingForces(PhysicsCmp& phyCmp, std::vector<Contact> contacts);


	void resolve_object(PhysicsCmp& phyCmp, RenderCmp &rndCmp, float t, const float dt, unsigned int iter);

public:
	PhysicsSys(EntitySys& ref0, TerrainSys& ref1);

	void update(float t, const float dt);

};