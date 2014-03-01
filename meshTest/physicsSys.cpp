#include "physicsSys.h"


State::State() : 
	p(0.0f, 0.0f, 0.0f),
	v(0.0f, 0.0f, 0.0f),
	w(0.0f, 0.0f, 0.0f) {}

State::State(PhysicsCmp &cmp) {
	p = cmp.p;
	v = cmp.v;
	w = cmp.w;
}

Derivative::Derivative() : 
	dx(0.0f, 0.0f, 0.0f),
	dv(0.0f, 0.0f, 0.0f),
	dw(0.0f, 0.0f, 0.0f) {}

Contact::Contact(glm::vec3 position, glm::vec3 radius, glm::vec3 normal, glm::vec3 rel_velocity, float penetration) : 
	p(position), 
	r(radius), 
	n(normal), 
	dp(rel_velocity), 
	pen(penetration) {}

PhysicsSys::PhysicsSys(EntitySys& ref0, TerrainSys& ref1) : 
	gravity(10.0f),
	drag(0.10f),
	entSys(ref0),
	terSys(ref1) {}


#pragma region RK4

//RK4 methods taken from http://gafferongames.com/game-physics/integration-basics/

Derivative PhysicsSys::evaluate(State& initial, float t, float dt, Derivative &deriv) {

	State state;
    state.p = initial.p + deriv.dx * dt;
    state.v = initial.v + deriv.dv * dt;
	state.w = initial.w + deriv.dw * dt;

    Derivative output;
    output.dx = state.v;
    output.dv = acceleration(state, t+dt);

	float b = drag;
	output.dw = glm::vec3(-b*state.w.x, -b*state.w.y, -b*state.w.z);
    return output;

}

glm::vec3 PhysicsSys::acceleration(State& state, float t) {

	float g = gravity;
	float b = drag;

	glm::vec3 res(-b*state.v.x, -g-b*state.v.y, -b*state.v.z);
	return res;

}

State PhysicsSys::integrate(State& state, float t, float dt) {

	Derivative a = evaluate(state, t, 0.0f,    Derivative());
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt,      c);

	glm::vec3 dxdt = 1.0f/6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
	glm::vec3 dvdt = 1.0f/6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
	glm::vec3 dwdt = 1.0f/6.0f * (a.dw + 2.0f * (b.dw + c.dw) + d.dw);
	
	State output;
	output.p = state.p + dxdt * dt;
	output.v = state.v + dvdt * dt;
	output.w = state.w + dwdt * dt;
	
	return output;

	/*
	cmp.p += cmp.v * dt;
	cmp.v += glm::vec3(-drag*cmp.v.x, -gravity-drag*cmp.v.y, -drag*cmp.v.z) * dt;
	cmp.w += glm::vec3(-drag*cmp.w.x, -drag*cmp.w.y, -drag*cmp.w.z) * dt;
	*/

}

#pragma endregion

#pragma region Ground_Functions

void PhysicsSys::clampAboveGround(PhysicsCmp& cmp) {
	
	//check if object is below ground
	float floor = terSys.getFloorHeight(cmp.p.x, cmp.p.z);
	if (cmp.p.y < floor) {
		cmp.p.y = floor;
		cmp.v.y = 0.0f;
	}

}

float PhysicsSys::ground(glm::vec3 vertex) {

	//check if object is below ground
	float floor = terSys.getFloorHeight(vertex.x, vertex.z);
	glm::vec3 n = terSys.getNormal(vertex.x, vertex.z);

	float pen = glm::dot(n, glm::vec3(0.0f, (floor - vertex.y), 0.0f));

	return pen;

}

float PhysicsSys::ground(glm::vec3 vertex, glm::vec3 normal) {

	//check if object is below ground
	float floor = terSys.getFloorHeight(vertex.x, vertex.z);

	float pen = glm::dot(normal, glm::vec3(0.0f, (floor - vertex.y), 0.0f));

	return pen;

}

#pragma endregion

#pragma region Update_Functions

void PhysicsSys::updateComponentState(PhysicsCmp& cmp, State& state) {
	
	cmp.p = state.p;
	cmp.v = state.v;
	cmp.w = state.w;

}

void PhysicsSys::updateComponentState(PhysicsCmp& cmp, RenderCmp &rndCmp, State& state, float dt) {

	cmp.p = state.p;
	cmp.v = state.v;
	cmp.w = state.w;

	glm::quat rotation = getQuaternionFromAngularVelocity(cmp.w, dt);
	cmp.o = rotation * cmp.o;

	//update matrices
	rndCmp.updateTranslationMatrix(cmp.p);
	rndCmp.updateRotationMatrix(cmp.o);
	rndCmp.updateModelMatrix();


}

#pragma endregion


glm::quat PhysicsSys::getQuaternionFromAngularVelocity(glm::vec3 ang_vel, float dt) {

	glm::quat rotation;

	float magn_w = glm::length(ang_vel * dt);
	if (magn_w > 0.0f) {

		float theta = 0.5f * magn_w;
		glm::vec3 norm_w = glm::normalize(ang_vel);

		float sinAngle = sin(theta);

		rotation = glm::quat(cos(theta), norm_w.x*sinAngle, norm_w.y*sinAngle, norm_w.z*sinAngle);

	}

	return rotation;

}


glm::mat3 PhysicsSys::getRotationMatrixFromQuaternion(glm::quat oreintation) {

	return glm::mat3_cast(oreintation);

}

void PhysicsSys::update(float t, const float dt) {

	PhysicsCmp* cmps = entSys.getAll<PhysicsCmp>();
	unsigned int size = entSys.getSize<PhysicsCmp>();

	//loop through entities
	for (unsigned int i = 0; i != size; ++i) {

		PhysicsCmp& phyCmp = cmps[i];
		ID exposedID = entSys.getExposedID<PhysicsCmp>(phyCmp.id);

		if ( entSys.has<RenderCmp>(exposedID) ) {
			RenderCmp &rndCmp = entSys.get<RenderCmp>(exposedID);
			
			//entity is not a simple point - must test for and resolve any collisions
			resolve_object(phyCmp, rndCmp, t, dt, 0);

		} else {
			//object is a simple point and will be placed above ground
			updateComponentState(phyCmp, integrate(State(phyCmp), t, dt));
			clampAboveGround(phyCmp);
		}

	}

}


std::vector<Contact> PhysicsSys::findGroundContacts(PhysicsCmp& phyCmp, RenderCmp& rndCmp, float t, float dt) {

	//find all contacts between the supplied object and the ground at t+dt

	std::vector<Contact> contacts;

	for (unsigned int k = 0; k != 8; ++k) {
		glm::vec4 localVertex(rndCmp.vertices[(3 * k) + 0], rndCmp.vertices[(3 * k) + 1], rndCmp.vertices[(3 * k) + 2], 1.0f);
		glm::vec4 worldVertex = rndCmp.modelMatrix * localVertex;
		glm::vec3 p0 = glm::vec3(worldVertex);
		glm::vec3 r = (p0 - phyCmp.p);
		glm::vec3 rel_v = (phyCmp.v + (glm::cross(phyCmp.w, r)));
		glm::vec3 p1 = p0 + rel_v * dt;

		glm::vec3 n = terSys.getNormal(p1.x, p1.z);

		float dy = ground(p1, n);
		if (dy > 0.0f) {
			//vertex is colliding with ground -> add collision information to container
			contacts.push_back(Contact(p1, r, n, rel_v, dy));
		}

	}

	return contacts;

}

int PhysicsSys::getContactType(Contact& c) {
	
	// -1	self-resolving
	//  0	resting
	//  1	colliding


	float perp_rel_v = glm::dot(c.n, c.dp);
	//printf("Contact: %f\n", perp_rel_v);

	float upper_threshold = 0.1f;
	float lower_threshold = -1.5f;
	if (perp_rel_v > upper_threshold) {
		//Contact is moving away on it's own
		return -1;
	}
	else if ((perp_rel_v > lower_threshold) && (perp_rel_v <= upper_threshold)) {
		//resting contact
		return 0;
	}
	else {
		//colliding contact
		return 1;
	}

}

float PhysicsSys::findTimeOfCollision(Contact c, float dt) {

	//return value b such that t + b*dt ~= time of collision

	//bisect method to find time when i_focus vertex is just barely penetrating
	float bisect = 0.5f; //percentage of dt to move forward

	glm::vec3 p0 = c.p;
	glm::vec3 n = c.n;
	glm::vec3 r = c.r;
	glm::vec3 rel_v = c.dp;

	unsigned int max_iter = 5;
	for (unsigned int m = 2; m <= max_iter; m++) {

		glm::vec3 p1 = p0 + rel_v*bisect*dt;

		if (ground(p1) > 0.0f) { bisect -= powf(0.5f, (float)m); } //if penetrating still, decrease bisect
		else { bisect += powf(0.5f, (float)m); }

	};

	glm::vec3 p1 = p0 + rel_v*bisect*dt;
	if (ground(p1) <= 0.0f) { bisect += powf(0.5f, (float)max_iter); } //if not penetrating, increase bisect to make sure we are
	p1 = p0 + rel_v*bisect*dt;
	if (ground(p1) <= 0.0f) { printf("c_focus is no longer penetrating!\n"); }

	return bisect;

}



std::vector<float> PhysicsSys::calcRestingForces(PhysicsCmp& phyCmp, std::vector<Contact> contacts) {

	unsigned int numberOfRestingContacts = contacts.size();

	//calculate 'A' matrix
	std::vector<std::vector<float>> matrixA;
	matrixA.reserve(numberOfRestingContacts);

	for (unsigned int n = 0; n != numberOfRestingContacts; ++n) {

		matrixA.push_back(std::vector<float>());
		matrixA[n].reserve(numberOfRestingContacts);

		for (unsigned int m = 0; m != numberOfRestingContacts; ++m) {

			glm::vec3 p0 = contacts[n].p;
			glm::vec3 n1 = contacts[n].n;
			glm::vec3 r1 = contacts[n].r;
			glm::vec3 n2 = contacts[m].n;
			glm::vec3 r2 = contacts[m].r;

			float mass = phyCmp.mass;
			glm::mat3 i_inv = phyCmp.invInertia;
			glm::mat3 rot_mat = getRotationMatrixFromQuaternion(phyCmp.o);

			glm::vec3 temp1 = glm::cross(rot_mat * (i_inv * (glm::inverse(rot_mat) * glm::cross(r2, n2))), r1);
			matrixA[n].push_back(glm::dot(n1, ((n2 / mass) + temp1)));

		}

	}

	//calculate 'b' vector
	std::vector<float> vectorB;
	vectorB.reserve(numberOfRestingContacts);
	for (unsigned int n = 0; n != numberOfRestingContacts; ++n) {

		glm::vec3 p0 = contacts[n].p;
		glm::vec3 n1 = contacts[n].n;
		glm::vec3 r1 = contacts[n].r;
		glm::vec3 w1 = phyCmp.w;
		glm::vec3 a1 = glm::vec3(0.0f, -gravity, 0.0f);

		glm::mat3 i_a = phyCmp.inertia;
		glm::mat3 i_inv = phyCmp.invInertia;
		glm::mat3 rot_mat = getRotationMatrixFromQuaternion(phyCmp.o);

		glm::vec3 k_v1 = glm::cross(w1, glm::cross(w1, r1)); //first velocity term
		glm::vec3 k_v2 = glm::cross((rot_mat * (i_inv * (glm::inverse(rot_mat) * glm::cross((rot_mat * (i_a * (glm::inverse(rot_mat) * w1))), w1)))), r1);

		//Special case: body 2 is immobile and there are no thrusters in this simulation
		vectorB.push_back(glm::dot(n1, (a1 + k_v1 + k_v2)));

	}

	//calculate acceleration and forces at each contact
	ContactSys contact(numberOfRestingContacts, matrixA, vectorB);
	return contact.getForces();

}

std::vector<float> PhysicsSys::calcCollidingForces(PhysicsCmp& phyCmp, std::vector<Contact> contacts) {
	
	std::vector<float> forces;

	for each (Contact c in contacts) {

		glm::vec3 n = c.n;
		glm::vec3 r = c.r;
		glm::vec3 rel_v = c.dp;

		//computation of impulse force
		float coeff_restitution = 0.5f;
		glm::mat3 rot_mat = getRotationMatrixFromQuaternion(phyCmp.o);
		
		float term1 = (1 / phyCmp.mass);
		float term2 = glm::dot(n, glm::cross((rot_mat * (phyCmp.invInertia * (glm::inverse(rot_mat) * glm::cross(r, n)))), r));

		float j = (-1.0f - coeff_restitution) * (glm::dot(rel_v, n)) / (term1 + term2);

		forces.push_back(j);

	}

	return forces;

}



void PhysicsSys::resolve_object(PhysicsCmp& phyCmp, RenderCmp &rndCmp, float t, const float dt, unsigned int iter) {

	//find all collisions that will occur at t+dt if nothing is done
	std::vector<Contact> contacts = findGroundContacts(phyCmp, rndCmp, t, dt);

	
	//if there are no collisions at t+dt, then just integrate and update the object
	if (contacts.size() == 0) {
		updateComponentState(phyCmp, rndCmp, integrate(State(phyCmp), t, dt), dt);
		return;
	}


	//if all collisions are resting contact we can just calculate resting forces
	bool allContactsAreResting = true;
	for each (Contact c in contacts) {
		if (getContactType(c) == 1) { allContactsAreResting = false; }
	}

	if (allContactsAreResting) {
		//resolve object as a resting contact
		//	solve for forces
		//	apply forces
		//	integrate
		//	update

		std::vector<float> forces = calcRestingForces(phyCmp, contacts);

		glm::vec3 result_force = glm::vec3();
		glm::vec3 result_torque = glm::vec3();

		for (unsigned int i = 0; i != contacts.size(); ++i) {

			glm::vec3 f = forces[i] * contacts[i].n;
			result_force += f;
			result_torque += glm::cross(((contacts[i].p + contacts[i].dp*dt) - phyCmp.p), f);

		}

		glm::mat3 rot_mat = getRotationMatrixFromQuaternion(phyCmp.o);

		phyCmp.v += result_force / phyCmp.mass;
		phyCmp.w += rot_mat * (phyCmp.invInertia * (glm::inverse(rot_mat) * result_torque));

		updateComponentState(phyCmp, rndCmp, integrate(State(phyCmp), t, dt), dt);
		return;

	}
	else {
		//if not all contacts are resting, back up the simulation until only one point is colliding
		//resolve that collision
		//	solve for forces
		//	apply forces
		//	integrate
		//	update

		std::vector<Contact>::const_iterator c_focus = contacts.cbegin(); //index of the vertex with the most penetration
		for (std::vector<Contact>::const_iterator c = contacts.cbegin(); c != contacts.cend(); c++) {
			if (c->pen > c_focus->pen) { c_focus = c; }
		}
		
		float bisect = findTimeOfCollision(*c_focus, dt);
		float adj_dt = bisect * dt;

		updateComponentState(phyCmp, rndCmp, integrate(State(phyCmp), t, adj_dt), adj_dt);

		std::vector<float> forces = calcCollidingForces(phyCmp, contacts);

		glm::vec3 result_delta_v;
		glm::vec3 result_delta_w;
		glm::mat3 rot_mat = getRotationMatrixFromQuaternion(phyCmp.o);
		for (unsigned int i = 0; i != forces.size(); ++i) {
			result_delta_v += forces[i] * contacts[i].n / phyCmp.mass;
			result_delta_w += rot_mat * (phyCmp.invInertia * (glm::inverse(rot_mat) * glm::cross(contacts[i].r, forces[i] * contacts[i].n)));
		}

		phyCmp.v += result_delta_v;
		phyCmp.w += result_delta_w;

		updateComponentState(phyCmp, rndCmp, integrate(State(phyCmp), t + adj_dt, dt - adj_dt), dt - adj_dt);
		return;

	}

}