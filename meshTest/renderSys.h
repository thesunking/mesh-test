#pragma once
#include <GL\glew.h>
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>
#include "renderCmp.h"
#include <Simple OpenGL Image Library\src\SOIL.h>


struct CubeDefinition {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> colors;
	std::vector<float> uvs;
	std::vector<float> tangents;
	std::vector<float> bitangents;
	std::vector<GLuint> indices;
	CubeDefinition();
};

class RenderSys {

private:

	sf::RenderWindow &window;
	unsigned int window_x, window_y;

	//main shader and its linked variables
	GLuint mainProgramID;
	GLuint modelMatrixID, rotationMatrixID, viewMatrixID, perspectMatrixID;
	GLuint lightID, normalmapSamplerID;


	//vertex array objects and vertex buffer objects
	GLuint terrainVAO, entityVAO;

	GLuint vertexBufferID, normalBufferID, colorBufferID, indexBufferID;
	GLuint uvBufferID, tangentBufferID, bitangentBufferID;
	GLuint normalmapTextureID;

	GLuint entity_VertexBufferID, entity_NormalBufferID, entity_ColorBufferID, entity_IndexBufferID;
	GLuint entity_uvBufferID, entity_tangentBufferID, entity_bitangentBufferID;
	GLuint entity_normalmapTextureID;
	

	unsigned int numberOfVertices;
	unsigned int numberOfIndices;

	glm::vec4 light;
	glm::mat4 mV; //view matrix
	glm::mat4 mP; //perspective matrix

	void loadShaders();


public:
	RenderSys(sf::RenderWindow &window);

	void prepare(); //called before any other draw functions for a given frame
	void updateViewMatrix( glm::mat4 matView );
	void updateLightPosition( float angle );

	void drawNumber(glm::vec3 pos, int number);

	void initTerrain(std::vector<glm::vec3> &points,
					std::vector<glm::vec3> &normals,
					std::vector<float> &color,
					std::vector<glm::vec2> &uvs,
					std::vector<glm::vec3> &tangents,
					std::vector<glm::vec3> &bitangents,
					std::vector<GLuint> &indices);
	void initEntity();


	void drawTerrain();
	void drawEntity(RenderCmp &cmp);

};