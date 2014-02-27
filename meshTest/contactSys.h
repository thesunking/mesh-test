#pragma once
#include <vector>
#include <limits>

#include <Eigen\Dense.h>

//functions used for resting contact force calculations
//taken from psuedo-code in David Braff's "Fast Contact Force Computation for Nonpenetrating Rigid Bodies"

//making this its own class to allow for future changes (such as the addition of friction calculations)

class ContactSys {

private:
	unsigned int n; //total number of contact points

	std::vector<unsigned int> set;
	//if set[i] = 1, i is a contacting point (a=0).		(in C)
	//if set[i] = 2, i is a non-contacting point (f=0).	(in NC)

	Eigen::MatrixXf matA; //matrix relating forces between contact points

	Eigen::VectorXf f; //forces at each contact point
	Eigen::VectorXf df; //direction of change for f
	Eigen::VectorXf a; //accelerations at each contact point
	Eigen::VectorXf da; //change in accelerations for each contact point
	Eigen::VectorXf b; //force-independent variables


	float s; //step-size
	unsigned int d; //index of contact with negative acceleration ("driving index")
	unsigned int j; //index of step-size limiting contact


	void calcForces(); //main loop for contactSys
	bool driveToZero(); //increases current contact's force until its acceleration is zero
	void fDirection(); //calculates df
	void maxStep(); //calculates maximum step size

	void debugPrint();

public:
	ContactSys(unsigned int number_of_contacts, std::vector<std::vector<float>> matA, std::vector<float> b);

	std::vector<float> getForces();

};