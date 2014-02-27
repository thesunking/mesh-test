#include "contactSys.h"



ContactSys::ContactSys(unsigned int number_of_contacts, std::vector<std::vector<float>> arg_matA, std::vector<float> arg_b) {
	
	n = number_of_contacts;

	matA = Eigen::MatrixXf::Zero(n, n);
	b = Eigen::VectorXf::Zero(n);
	f = Eigen::VectorXf::Zero(n);
	df = Eigen::VectorXf::Zero(n);
	a = Eigen::VectorXf::Zero(n);
	da = Eigen::VectorXf::Zero(n);

	//prep the vectors
	for (unsigned int i = 0; i != n; ++i) {
		set.push_back(0);
	}

	for (unsigned int i = 0; i != n; ++i) {
		for (unsigned int j = 0; j != n; ++j) {
			matA(i, j) = arg_matA[i][j];
		}
	}

	for (unsigned int i = 0; i != n; ++i) {
		b(i) = arg_b[i];
	}

}

void ContactSys::calcForces() {

	a = b;
	d = 0;

	/*
	bool anyAccelsNegative = true;
	while (anyAccelsNegative) {

		anyAccelsNegative = false;
		for (unsigned int i = 0; i != a.size(); ++i) {
			if ((a[i] < 0.0f) && !anyAccelsNegative) {
				anyAccelsNegative = true;
				d = i;

				bool returnFlag = true;
				while (returnFlag) { returnFlag = driveToZero(); }
				printf("LOOPING\n");
			}
		}

	}*/

	debugPrint();

	for (unsigned int i = 0; i != n; ++i) {
		d = i;
		bool returnFlag = true;
		while (returnFlag) { returnFlag = driveToZero(); }
		printf("LOOPING\n");
	}

	

}

bool ContactSys::driveToZero() {

	//return true -> re-execute
	//return false -> continue

	printf("Drive to Zero\n");

	fDirection();
	da = matA * df;

	maxStep(); //updates s and j
	f = f + s*df;
	a = a + s*da;

	debugPrint();


	if (set[j] == 1) {
		//if j is in contacting set
		//move j into non-contacting set
		set[j] = 2;
		return true; //re-execute
	}
	else if (set[j] == 2) {
		//if j is in non-contacting set
		//move j into contacting set
		set[j] = 1;
		return true; //re-execute
	}
	else {
		//j has not been assigned yet
		//add j to contacting set
		set[j] = 1;
		return false; //continue
	}



}

void ContactSys::fDirection() {

	//zero out df
	for (unsigned int i = 0; i != n; ++i) {
		df[i] = 0.0f;
	}

	df[d] = 1.0f;

	std::vector<unsigned int> indicesInC;
	for (unsigned int i = 0; i != n; ++i) {
		if (set[i] == 1) { indicesInC.push_back(i); }
	}

	unsigned int indSize = indicesInC.size();

	if (indSize == 0) {
		printf("Set C is empty!\n");
		return;
	}

	//A11 = Acc
	Eigen::MatrixXf matAcc = Eigen::MatrixXf::Zero(indSize, indSize);
	for (unsigned int i = 0; i != indSize; ++i) {
		for (unsigned int j = 0; j != indSize; ++j) {
			matAcc(i, j) = matA(indicesInC[i], indicesInC[j]);
		}
	}

	//v1 = Acd
	Eigen::VectorXf vecAcd = Eigen::VectorXf::Zero(indSize);
	for (unsigned int i = 0; i != indSize; ++i) {
		vecAcd(i) = -matA(indicesInC[i], d);
	}

	//solve A11*x=-v1
	//Eigen::MatrixXf x = matAcc.colPivHouseholderQr().solve(vecAcd);
	Eigen::MatrixXf x = matAcc.ldlt().solve(vecAcd);

	//check solution
	float relative_error = (matAcc*x - vecAcd).norm() / vecAcd.norm();
	printf("Rel Err in Contact Soln: %f\n", relative_error);

	//transfer x into df
	for (unsigned int i = 0; i != indSize; ++i) {
		df( indicesInC[i] ) = x(i);
	}



}

void ContactSys::maxStep() {

	s = std::numeric_limits<float>::max();
	j = -1;

	if (da[d] > 0.0f) {
		j = d;
		s = -a[d] / da[d];
	}

	for (unsigned int i = 0; i != n; ++i) {
		
		if ((set[i] == 1) && (df[i] < 0.0f)) {

			float temp_s = -f[i] / df[i];
			if (temp_s < s) {
				s = temp_s;
				j = i;
			}

		}

		if ((set[i] == 2) && (da[i] < 0.0f)) {

			float temp_s = -a[i] / da[i];
			if (temp_s < s) {
				s = temp_s;
				j = i;
			}


		}

	}

}

std::vector<float> ContactSys::getForces() {

	calcForces();

	std::vector<float> temp_f;

	for (unsigned int i = 0; i != n; ++i) {
		temp_f.push_back(f[i]);
	}

	return temp_f;

}

void ContactSys::debugPrint() {

	//Debug prints
	printf("d: %i   j: %i   s: %f\n", d, j, s);
	printf("st: "); for (unsigned int i = 0; i != n; ++i) { printf("%i, ", set[i]); } printf("\n");
	printf(" f: "); for (unsigned int i = 0; i != n; ++i) { printf("%f, ", f[i]); } printf("\n");
	printf("df: "); for (unsigned int i = 0; i != n; ++i) { printf("%f, ", df[i]); } printf("\n");
	printf(" a: "); for (unsigned int i = 0; i != n; ++i) { printf("%f, ", a[i]); } printf("\n");
	printf("da: "); for (unsigned int i = 0; i != n; ++i) { printf("%f, ", da[i]); } printf("\n");

}