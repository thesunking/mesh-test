#include "worldSys.h"
//#include "SFML\OpenGL.hpp"

WorldSys::WorldSys(RenderSys &render) : control(entity, terrain, camera), camera(entity), physics(entity, terrain), sunAngle(1.5708f) {

	//create the player entity and pass its id to control & camera
	ID id = entity.createEntity();
	entity.add<PhysicsCmp>(id, PhysicsCmp());
	entity.get<PhysicsCmp>(id).p = glm::vec3(320.0f, 75.0f, 320.0f);

	control.setPlayerID(id);
	camera.setPlayerID(id);


	testBlockID = entity.createPrefab();
	
	float local_vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f
	};
	unsigned int local_indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		2, 3, 7,
		2, 7, 6,
		3, 0, 4,
		3, 4, 7,
		4, 5, 6,
		4, 6, 7
	};

	entity.addPrefabCmp(testBlockID, PhysicsCmp());
	entity.addPrefabCmp(testBlockID, RenderCmp(local_vertices, local_indices));
	
	ID block1 = entity.createEntity(testBlockID);
	entity.get<PhysicsCmp>(block1).p = glm::vec3( 341.5f, 75.0f, 327.8f );
	entity.get<PhysicsCmp>(block1).updateInertiaMatrix( 10.0f, 5.0f, 5.0f );
	entity.get<RenderCmp>(block1).scalingMatrix = glm::scale(10.0f, 5.0f, 5.0f);
	entity.get<RenderCmp>(block1).updateTranslationMatrix( entity.get<PhysicsCmp>(block1).p );
	entity.get<RenderCmp>(block1).updateModelMatrix();

	/*
	ID block2 = entity.createEntity(testBlockID);
	entity.get<PhysicsCmp>(block2).p = glm::vec3(421.1f, 75.0f, 207.2f);
	entity.get<PhysicsCmp>(block2).updateInertiaMatrix( 5.0f, 10.0f, 1.0f );
	entity.get<RenderCmp>(block2).scalingMatrix = glm::scale( 5.0f, 5.0f, 5.0f );
	entity.get<RenderCmp>(block2).updateTranslationMatrix(entity.get<PhysicsCmp>(block2).p);
	entity.get<RenderCmp>(block2).updateModelMatrix();
	*/

	render.initFramebuffer();
	render.initTerrain(terrain.getPoints(), terrain.getNormals(), terrain.getColors(),terrain.getUVs(), terrain.getTangents(), terrain.getBitangents(), terrain.getIndices());
	render.initEntity();

}

gameState WorldSys::update(InputResponse data, sf::Window &window, float t, const float dt) {

	// - special thanks to -
    // Maeve and Andrew for being B-B-B-Bosses.

	sunAngle += 0.005f;
	control.update(data);
	physics.update(t, dt);

	//reset mouse to center
	sf::Vector2u size = window.getSize();
	sf::Mouse::setPosition(sf::Vector2i(size.x * 0.5, size.y * 0.5), window);

	return GAME;

}

void WorldSys::draw(RenderSys &render) {

	render.updateViewMatrix(camera.updateView());
	render.updateLightPosition(sunAngle);

	// RenderSys is treated as a state machine (similar to OpenGL)
	// Before drawing objects, WorldSys will put RenderSys into the appropriate state

	// Clear shadowmap from previous frame
	render.clearShadowmap();

	// Render everything to shadowmap
	render.state(renderState::RENDER_TO_SHADOWMAP);
	//drawAll(render);
	RenderCmp* cmps = entity.getAll<RenderCmp>();
	unsigned int size = entity.getSize<RenderCmp>();

	for (unsigned int i = 0; i != size; ++i) {
		render.drawEntity(cmps[i]);
	}

	// Render everything to screen
	render.state(renderState::RENDER_TO_SCREEN);
	drawAll(render);
	
}

void WorldSys::drawAll(RenderSys &render) {

	render.drawTerrain();

	RenderCmp* cmps = entity.getAll<RenderCmp>();
	unsigned int size = entity.getSize<RenderCmp>();

	for (unsigned int i = 0; i != size; ++i) {
		render.drawEntity(cmps[i]);
	}

}
