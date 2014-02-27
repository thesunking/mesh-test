#pragma once
#include "lookupID.h"
#include <array>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\gtc\quaternion.hpp>

class RenderCmp {

public:
	ID id;

	float vertices[24];
	unsigned int indices[36];
	float colors[24];

	glm::mat4 scalingMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 modelMatrix;
	
	RenderCmp();
	RenderCmp(float* vertices, unsigned int* indices);

	void updateScalingMatrix(glm::vec3 scalar);
	void updateRotationMatrix(glm::vec3 rotation_axis, float angle);
	void updateTranslationMatrix(glm::vec3 position);

	void updateRotationMatrix(glm::quat quaternion);

	void updateModelMatrix();

};