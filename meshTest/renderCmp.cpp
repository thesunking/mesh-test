#include "renderCmp.h"

RenderCmp::RenderCmp() {

	scalingMatrix	  = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	rotationMatrix    = glm::rotate( 0.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	translationMatrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	for (unsigned int i = 0; i != 24; ++i) {
		colors[i] = 0.5f;
	}

}

RenderCmp::RenderCmp(float* vrt, unsigned int* ind) {

	for (unsigned int i = 0; i != 24; ++i) {
		vertices[i] = vrt[i];
	}

	for (unsigned int i = 0; i != 24; ++i) {
		colors[i] = 0.5f;
	}

	for (unsigned int i = 0; i != 36; ++i) {
		indices[i] = ind[i];
	}

	scalingMatrix = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
	rotationMatrix    = glm::rotate( 0.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	translationMatrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

}

void RenderCmp::updateScalingMatrix(glm::vec3 scalar) { scalingMatrix = glm::scale(glm::vec3(scalar.x, scalar.y, scalar.z)); }

void RenderCmp::updateRotationMatrix(glm::vec3 rotation_axis, float angle) { rotationMatrix = glm::rotate( angle, glm::vec3(rotation_axis.x, rotation_axis.y, rotation_axis.z) ); }
void RenderCmp::updateRotationMatrix(glm::quat quaternion) { rotationMatrix = glm::mat4_cast( quaternion ); }

void RenderCmp::updateTranslationMatrix(glm::vec3 position) { translationMatrix = glm::translate(position); }

void RenderCmp::updateModelMatrix() { modelMatrix = translationMatrix * rotationMatrix * scalingMatrix; }