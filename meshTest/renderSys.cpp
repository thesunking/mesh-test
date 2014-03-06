#include "renderSys.h"
#include "loadShader.h"
#include <sstream>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))


CubeDefinition::CubeDefinition()
{
	vertices = {
		-0.5f, -0.5f, -0.5f,
		+0.5f, -0.5f, -0.5f,
		+0.5f, -0.5f, +0.5f,
		-0.5f, -0.5f, -0.5f,
		+0.5f, -0.5f, +0.5f,
		-0.5f, -0.5f, +0.5f,

		-0.5f, -0.5f, +0.5f,
		+0.5f, -0.5f, +0.5f,
		+0.5f, +0.5f, +0.5f,
		-0.5f, -0.5f, +0.5f,
		+0.5f, +0.5f, +0.5f,
		-0.5f, +0.5f, +0.5f,

		+0.5f, -0.5f, +0.5f,
		+0.5f, -0.5f, -0.5f,
		+0.5f, +0.5f, -0.5f,
		+0.5f, -0.5f, +0.5f,
		+0.5f, +0.5f, -0.5f,
		+0.5f, +0.5f, +0.5f,

		+0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, +0.5f, -0.5f,
		+0.5f, -0.5f, -0.5f,
		-0.5f, +0.5f, -0.5f,
		+0.5f, +0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, +0.5f,
		-0.5f, +0.5f, +0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, +0.5f, +0.5f,
		-0.5f, +0.5f, -0.5f,

		-0.5f, +0.5f, +0.5f,
		+0.5f, +0.5f, +0.5f,
		+0.5f, +0.5f, -0.5f,
		-0.5f, +0.5f, +0.5f,
		+0.5f, +0.5f, -0.5f,
		-0.5f, +0.5f, -0.5f
	};

	normals = {
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	colors = {
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f
	};

	uvs = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f

	};

	tangents = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	bitangents = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};


	indices = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};
}


RenderSys::RenderSys(sf::RenderWindow &w) : window(w), window_x(w.getSize().x), window_y(w.getSize().y)
{
	glewInit();

	mP = glm::perspective(90.0f, 1.0f, 0.1f, 1000.0f);

	light = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);


	//depth & color initialization
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	//load shaders
	loadShaders();
	glUseProgram(mainProgramID);
}

void RenderSys::loadShaders()
{
	//main shader and linked variables
	mainProgramID = LoadShaders("res/main.vert", "res/main.frag");

	// vertex shader - uniforms
	modelMatrixID = glGetUniformLocation(mainProgramID, "mM");
	rotationMatrixID = glGetUniformLocation(mainProgramID, "mR");
	viewMatrixID = glGetUniformLocation(mainProgramID, "mV");
	perspectMatrixID = glGetUniformLocation(mainProgramID, "mP");

	//fragment shader - uniforms
	lightID = glGetUniformLocation(mainProgramID, "vLight");
	normalmapSamplerID = glGetUniformLocation(mainProgramID, "NormalTextureSampler");
}

void RenderSys::prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSys::updateViewMatrix(glm::mat4 matView)
{
	mV = matView;
}

void RenderSys::updateLightPosition(float angle)
{
	float temp_sin = sinf(angle);
	float temp_cos = cosf(angle);

	//glm::vec3 temp_light = glm::vec3(0.0f, sinf(angle), cosf(angle));
	//temp_light = glm::normalize(temp_light);
	light[0] = 0.0f;
	light[1] = temp_sin;
	light[2] = temp_cos;
	light[3] = 1.0f;

	//light *= 2.0f;

	glm::vec3 sky = temp_sin * glm::vec3(0.47f, 0.82f, 1.00f);
	glClearColor(sky.x, sky.y, sky.z, 0.0f);
}

void RenderSys::drawNumber(glm::vec3 pos, int number)
{
	//Print framerate
	printf("%d\n", number);
}


#pragma region genArrayBuffer

GLuint RenderSys::genArrayBuffer(GLuint location, std::vector<glm::vec2> &data)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return bufferID;
}

GLuint RenderSys::genArrayBuffer(GLuint location, std::vector<glm::vec3> &data)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return bufferID;
}

GLuint RenderSys::genArrayBuffer(GLuint location, std::vector<glm::vec4> &data)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return bufferID;
}

GLuint RenderSys::genArrayBuffer(GLuint location, std::vector<float> &data, GLuint stride)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, stride, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return bufferID;
}

#pragma endregion

GLuint RenderSys::genElementBuffer(std::vector<GLuint> &indices)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	return bufferID;
}


void RenderSys::initTerrain(std::vector<glm::vec3> &vertexColl, std::vector<glm::vec3> &normalColl, std::vector<float> &color, std::vector<glm::vec2> &uvColl, std::vector<glm::vec3> &tangentColl, std::vector<glm::vec3> &bitangentColl, std::vector<GLuint> &indices)
{
	numberOfVertices = vertexColl.size();
	numberOfIndices = indices.size();

	//this gives normals for each vertex
	std::vector<glm::vec3> normals;
	unsigned int side_length = (unsigned int)sqrt(numberOfVertices);
	for (unsigned int x = 0; x != side_length; ++x)
	{
		for (unsigned int z = 0; z != side_length; ++z)
		{

			bool onEdge_LowX = (x == 0);
			bool onEdge_LowZ = (z == 0);
			bool onEdge_HighX = (x == side_length - 1);
			bool onEdge_HighZ = (z == side_length - 1);

			glm::vec3 result = glm::vec3();

			if ((!onEdge_LowX) && (!onEdge_LowZ))
			{
				//result += tri_nrm_0 + tri_nrm_1;
				unsigned int i0 = 2 * ((x - 1) * (side_length - 1) + (z - 1));
				unsigned int i1 = i0 + 1;

				result += normalColl[i0] + normalColl[i1];
			}

			if ((!onEdge_LowX) && (!onEdge_HighZ))
			{
				//result += tri_nrm_2;
				unsigned int i2 = 2 * ((x - 1) * (side_length - 1) + z);

				result += normalColl[i2];
			}

			if ((!onEdge_HighX) && (!onEdge_HighZ))
			{
				//result += tri_nrm_3 + tri_nrm_4;
				unsigned int i4 = 2 * (x * (side_length - 1) + z);
				unsigned int i3 = i4 + 1;

				result += normalColl[i3] + normalColl[i4];
			}

			if ((!onEdge_HighX) && (!onEdge_LowZ))
			{
				//result += tri_nrm_5;
				unsigned int i5 = 2 * (x * (side_length - 1) + (z - 1)) + 1;

				result += normalColl[i5];
			}

			result = glm::normalize(result);
			normals.push_back(result);

		}
	}

	//add alpha values to color array
	std::vector<glm::vec4> colors;
	for (unsigned int i = 0; i < (color.size() / 3); ++i)
	{
		colors.push_back(glm::vec4(color[3 * i], color[3 * i + 1], color[3 * i + 2], 0.5f));
	}


	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenTextures(1, &normalmapTextureID);
	glBindTexture(GL_TEXTURE_2D, normalmapTextureID);

	int width = 1024, height = 1024;
	unsigned char* image = SOIL_load_image("res/nrmStone.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	genArrayBuffer(0, vertexColl);
	genArrayBuffer(1, normals);
	genArrayBuffer(2, colors);
	genArrayBuffer(3, uvColl);
	genArrayBuffer(4, tangentColl);
	genArrayBuffer(5, bitangentColl);

	genElementBuffer(indices);
}

void RenderSys::initEntity()
{
	glGenVertexArrays(1, &entityVAO);
	glBindVertexArray(entityVAO);

	glGenTextures(1, &entity_normalmapTextureID);
	glBindTexture(GL_TEXTURE_2D, entity_normalmapTextureID);

	int width = 512, height = 512;
	unsigned char* image = SOIL_load_image("res/nrmWood.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	CubeDefinition cube;
	genArrayBuffer(0, cube.vertices, 3);
	genArrayBuffer(1, cube.normals, 3);
	genArrayBuffer(2, cube.colors, 4);
	genArrayBuffer(3, cube.uvs, 2);
	genArrayBuffer(4, cube.tangents, 3);
	genArrayBuffer(5, cube.bitangents, 3);

	genElementBuffer(cube.indices);
}


void RenderSys::drawTerrain()
{
	glBindVertexArray(terrainVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normalmapTextureID);
	glUniform1i(normalmapSamplerID, 0);


	glm::mat4 mM = glm::mat4(); //create identity matrix to stand in for model matrix (since terrain vertices are already in world-space)
	glm::mat4 mR = glm::mat4(); //ditto (don't want to change normals)
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &mM[0][0]);
	glUniformMatrix4fv(rotationMatrixID, 1, GL_FALSE, &mR[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &mV[0][0]);
	glUniformMatrix4fv(perspectMatrixID, 1, GL_FALSE, &mP[0][0]);
	glUniform4fv(lightID, 1, &light[0]);

	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, (void*)0);
}

void RenderSys::drawEntity(RenderCmp &cmp)
{
	glBindVertexArray(entityVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, entity_normalmapTextureID);
	glUniform1i(normalmapSamplerID, 0);


	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &cmp.modelMatrix[0][0]);
	glUniformMatrix4fv(rotationMatrixID, 1, GL_FALSE, &cmp.rotationMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &mV[0][0]);
	glUniformMatrix4fv(perspectMatrixID, 1, GL_FALSE, &mP[0][0]);
	glUniform4fv(lightID, 1, &light[0]);


	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
}