#include "renderSys.h"
#include "loadShader.h"
#include <sstream>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))


CubeDefinition::CubeDefinition() {

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


RenderSys::RenderSys(sf::RenderWindow &w) : window(w), window_x(w.getSize().x), window_y(w.getSize().y) {

	glewInit();

	mP = glm::perspective(90.0f, 1.0f, 0.1f, 1000.0f);

	light = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	//depth & color initialization
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	//load shaders
	loadShaders();

}

void RenderSys::loadShaders() {

	//main shader and linked variables
	mainProgramID = LoadShaders("res/main.vert", "res/main.frag");

	// vertex shader - uniforms
	modelMatrixID = glGetUniformLocation(mainProgramID, "mM");
	rotationMatrixID = glGetUniformLocation(mainProgramID, "mR");
	viewMatrixID = glGetUniformLocation(mainProgramID, "mV");
	perspectMatrixID = glGetUniformLocation(mainProgramID, "mP");
	//invLightID = glGetUniformLocation(mainProgramID, "invLight");
	depthBiasMVPID = glGetUniformLocation(mainProgramID, "depthBiasMVP");

	//fragment shader - uniforms
	lightID = glGetUniformLocation(mainProgramID, "vLight");
	normalmapSamplerID = glGetUniformLocation(mainProgramID, "NormalTextureSampler");
	shadowmapSamplerID = glGetUniformLocation(mainProgramID, "ShadowmapSampler");
	
	
	
	//shadowmap shader and linked variables
	shadowmapProgramID = LoadShaders("res/shadowmap.vert", "res/shadowmap.frag");
	depthMVPID = glGetUniformLocation(shadowmapProgramID, "depthMVP");


}

void RenderSys::prepare() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void RenderSys::state(renderState state) {

	switch (state)
	{
	case renderState::RENDER_TO_SCREEN:

		currentState = renderState::RENDER_TO_SCREEN;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window_x, window_y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(mainProgramID);
		

		break;
	case renderState::RENDER_TO_SHADOWMAP:

		currentState = renderState::RENDER_TO_SHADOWMAP;
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(0, 0, window_x, window_y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shadowmapProgramID);
		

		break;
	default:
		printf("Warning: Unhandled case in RenderSys::state\n");
		break;
	}

}

void RenderSys::clearShadowmap() {

	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, window_x, window_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

}

void RenderSys::updateViewMatrix(glm::mat4 matView) { mV = matView; }

void RenderSys::updateLightPosition(float angle) {

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

void RenderSys::drawNumber(glm::vec3 pos, int number) {
	
	//Print framerate
	//printf("%d\n", number);

	/*
	window.pushGLStates();
	//glPushMatrix();
		
		
		
		std::ostringstream ss;
		ss << number;
		sf::Text text;
		
		text.setFont(gameFont);
		text.setString(ss.str());
		text.setCharacterSize(25);
		text.setColor(sf::Color::White);
		text.setStyle(sf::Text::Regular);
		text.setPosition(pos.x,pos.y);

		window.draw(text);

	//glPopMatrix();
	window.popGLStates();
	*/

}



void RenderSys::initFramebuffer() {

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, window_x, window_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { printf("Warning: Framebuffer not initialized.\n"); }

}

void RenderSys::initTerrain(std::vector<glm::vec3> &vertexColl, std::vector<glm::vec3> &normalColl, std::vector<float> &color, std::vector<glm::vec2> &uvColl, std::vector<glm::vec3> &tangentColl, std::vector<glm::vec3> &bitangentColl, std::vector<GLuint> &indices) {
	
	numberOfVertices = vertexColl.size();
	numberOfIndices = indices.size();

	//this gives normals for each vertex
	std::vector<glm::vec3> normals;
	unsigned int side_length = (unsigned int)sqrt(numberOfVertices);
	for (unsigned int m = 0; m < side_length; ++m) {
		for (unsigned int n = 0; n < side_length; ++n) {

			unsigned int m0 = m;
			unsigned int n0 = n;
			if (m0 == side_length-1) {m0 -= 1;}
			if (n0 == side_length-1) {n0 -= 1;}

			unsigned int index = 2*(m0*(side_length-1)+n0);

			normals.push_back(normalColl[index]);

		}
	}

	//add alpha values to color array
	std::vector<glm::vec4> colors;
	for (unsigned int i = 0; i < (color.size() / 3); ++i) {
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

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexColl.size()*sizeof(glm::vec3), &vertexColl[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvColl.size() * sizeof(glm::vec2), &uvColl[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &tangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, tangentColl.size() * sizeof(glm::vec3), &tangentColl[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &bitangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, bitangentColl.size() * sizeof(glm::vec3), &bitangentColl[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


}

void RenderSys::initEntity() {

	CubeDefinition cube;
	std::vector<float> local_vertices = cube.vertices;
	std::vector<float> local_normals = cube.normals;
	std::vector<float> local_colors = cube.colors;
	std::vector<float> local_uvs = cube.uvs;
	std::vector<float> local_tangents = cube.tangents;
	std::vector<float> local_bitangents = cube.bitangents;
	std::vector<GLuint> local_indices = cube.indices;
	
	unsigned int numVert = 36;

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

	glGenBuffers(1, &entity_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 3 * numVert*sizeof(float), &local_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_NormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_NormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, 3 * numVert*sizeof(float), &local_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_ColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_ColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * numVert*sizeof(float), &local_colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, local_uvs.size() * sizeof(float), &local_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_tangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_tangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, local_tangents.size() * sizeof(float), &local_tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_bitangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, entity_bitangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, local_bitangents.size() * sizeof(float), &local_bitangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &entity_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, local_indices.size()*sizeof(GLuint), &local_indices[0], GL_STATIC_DRAW);



}





void RenderSys::drawTerrain() {

	glBindVertexArray(terrainVAO);


	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(light), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;


	if (currentState == renderState::RENDER_TO_SCREEN)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, normalmapTextureID);
		glUniform1i(normalmapSamplerID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(shadowmapSamplerID, 1);

		glm::mat4 mM = glm::mat4(); //create identity matrix to stand in for model matrix (since terrain vertices are already in world-space)
		glm::mat4 mR = glm::mat4(); //ditto (don't want to change normals)
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &mM[0][0]);
		glUniformMatrix4fv(rotationMatrixID, 1, GL_FALSE, &mR[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &mV[0][0]);
		glUniformMatrix4fv(perspectMatrixID, 1, GL_FALSE, &mP[0][0]);
		glUniform4fv(lightID, 1, &light[0]);

		glUniformMatrix4fv(depthBiasMVPID, 1, GL_FALSE, &depthBiasMVP[0][0]);
	}
	else if (currentState == renderState::RENDER_TO_SHADOWMAP)
	{
		glUniformMatrix4fv(depthMVPID, 1, GL_FALSE, &depthMVP[0][0]);
	}
	else
	{
		printf("Warning: Unhandled state in RenderSys::drawTerrain().\n");
	}


	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, (void*)0);

}



void RenderSys::drawEntity(RenderCmp &cmp) {

	glBindVertexArray(entityVAO);

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(light), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	if (currentState == renderState::RENDER_TO_SCREEN)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity_normalmapTextureID);
		glUniform1i(normalmapSamplerID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(shadowmapSamplerID, 1);

		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &cmp.modelMatrix[0][0]);
		glUniformMatrix4fv(rotationMatrixID, 1, GL_FALSE, &cmp.rotationMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &mV[0][0]);
		glUniformMatrix4fv(perspectMatrixID, 1, GL_FALSE, &mP[0][0]);
		glUniform4fv(lightID, 1, &light[0]);

		glUniformMatrix4fv(depthBiasMVPID, 1, GL_FALSE, &depthBiasMVP[0][0]);
	}
	else if (currentState == renderState::RENDER_TO_SHADOWMAP)
	{
		glUniformMatrix4fv(depthMVPID, 1, GL_FALSE, &depthMVP[0][0]);
	}
	else
	{
		printf("Warning: Unhandled state in RenderSys::drawTerrain().\n");
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	

}


/*void RenderSys::loadAssets() {

	//load font
	if (!gameFont.loadFromFile("res/TIMES.ttf")) { printf("RenderSys::loadAssets - Error - Could not load font\n"); }

}*/


/*//n = number of triangles
unsigned int n = local_indices.size() / 3;
std::vector<float> local_tangents;
std::vector<float> local_bitangents;
for (unsigned int i = 0; i < n; ++i) {

glm::vec3 deltaPos1 = glm::vec3(
local_vertices[3 * (local_indices[3 * i + 2])] -	 local_vertices[3 * (local_indices[3 * i])],
local_vertices[3 * (local_indices[3 * i + 2]) + 1] - local_vertices[3 * (local_indices[3 * i]) + 1],
local_vertices[3 * (local_indices[3 * i + 2]) + 2] - local_vertices[3 * (local_indices[3 * i]) + 2]
);

glm::vec3 deltaPos2 = glm::vec3(
local_vertices[3 * (local_indices[3 * i + 1])] -	 local_vertices[3 * (local_indices[3 * i])],
local_vertices[3 * (local_indices[3 * i + 1]) + 1] - local_vertices[3 * (local_indices[3 * i]) + 1],
local_vertices[3 * (local_indices[3 * i + 1]) + 2] - local_vertices[3 * (local_indices[3 * i]) + 2]
);

glm::vec2 deltaUV1 = glm::vec2(
local_uvs[2 * (local_indices[3 * i + 2])] -		local_uvs[2 * (local_indices[3 * i])],
local_uvs[2 * (local_indices[3 * i + 2]) + 1] - local_uvs[2 * (local_indices[3 * i]) + 1]
);

glm::vec2 deltaUV2 = glm::vec2(
local_uvs[2 * (local_indices[3 * i + 1])] -		local_uvs[2 * (local_indices[3 * i])],
local_uvs[2 * (local_indices[3 * i + 1]) + 1] - local_uvs[2 * (local_indices[3 * i]) + 1]
);

float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

local_tangents.push_back(tangent.x);
local_tangents.push_back(tangent.y);
local_tangents.push_back(tangent.z);
local_bitangents.push_back(bitangent.x);
local_bitangents.push_back(bitangent.y);
local_bitangents.push_back(bitangent.z);

local_tangents.push_back(tangent.x);
local_tangents.push_back(tangent.y);
local_tangents.push_back(tangent.z);
local_bitangents.push_back(bitangent.x);
local_bitangents.push_back(bitangent.y);
local_bitangents.push_back(bitangent.z);

local_tangents.push_back(tangent.x);
local_tangents.push_back(tangent.y);
local_tangents.push_back(tangent.z);
local_bitangents.push_back(bitangent.x);
local_bitangents.push_back(bitangent.y);
local_bitangents.push_back(bitangent.z);

}*/