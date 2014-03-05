#include "physicsCmp.h"

PhysicsCmp::PhysicsCmp() :
	mass(10.0f),
	inertia( glm::mat3(glm::scale(glm::vec3(mass/12.0f, mass/12.0f, mass/12.0f))) ),
	invInertia( glm::mat3(glm::inverse(inertia)) ),
	p(0.0f, 0.0f, 0.0f),
	v(0.0f, 0.0f, 0.0f),
	w(0.0f, 0.0f, 0.0f),
	o(1.0f, 0.0f, 0.0f, 0.0f)
{
}

PhysicsCmp::PhysicsCmp(glm::vec3 position) :
	mass(10.0f),
	inertia(glm::mat3(glm::scale(glm::vec3(mass / 12.0f, mass / 12.0f, mass / 12.0f)))),
	invInertia( glm::mat3(glm::inverse(inertia)) ),
	p(position),
	v(0.0f, 0.0f, 0.0f),
	w(0.0f, 0.0f, 0.0f),
	o(1.0f, 0.0f, 0.0f, 0.0f)
{
}

void PhysicsCmp::updateInertiaMatrix( float x, float y, float z )
{
	//https://en.wikipedia.org/wiki/List_of_moment_of_inertia_tensors

	float a = (mass/12.0f);
	inertia = glm::mat3(glm::scale(glm::vec3(a*(y*y + z*z), a*(x*x + z*z), a*(x*x + y*y))));
	invInertia = glm::mat3( glm::inverse( inertia ) );
}