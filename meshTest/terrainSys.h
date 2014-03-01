#pragma once
#include <array>
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "perlinNoise.h"

class TerrainSys {

private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<float> arrTemp;
	std::vector<float> arrRain;
	std::vector<float> arrColor;
	std::vector<GLuint> indices;		//3 GLuint per triangle

	std::vector<glm::vec2> uvs;			//1 glm::vec2 per vertex
	std::vector<glm::vec3> tangents;	//1 glm::vec3 per triangle
	std::vector<glm::vec3> bitangents;	//1 glm::vec3 per triangle

	std::vector<glm::vec3> avg_tan;		//1 glm::vec3 per vertex
	std::vector<glm::vec3> avg_bitan;	//1 glm::vec3 per vertex


	const unsigned int side_length;
	const float scaleAmount;

	PerlinNoise height0;
	PerlinNoise height1;
	PerlinNoise height2;
	PerlinNoise temp;
	PerlinNoise rain;

	float getHeight(float x, float z);

	void createPerlinMesh();
	
	void createHeightmap();
	void scaleMesh();
	void createIndices();
	void createNormals();
	void addTempAndRain();
	void coolHighPoints();
	void addRainShadows();
	void colorBiomes();
	

	void calcTextureCoords();
	void calcTangents();



public:
	TerrainSys();

	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getNormals();
	std::vector<float> getColors();
	std::vector<GLuint> getIndices();
	std::vector<glm::vec2> getUVs();
	std::vector<glm::vec3> getTangents();
	std::vector<glm::vec3> getBitangents();

	void cleanUp(); //cleans up arrays that are only used during init

	glm::vec2 getBarycentricCoords(unsigned int triIndex, float x, float z);
	unsigned int getTriangleIndex(float x, float z);
	float getFloorHeight(float x, float z);
	glm::vec3 getNormal(float x, float z);

	//void draw(RenderSys &render);

};