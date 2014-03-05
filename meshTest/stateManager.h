#pragma once
#include <deque>
#include "typedefs.h"
#include "menuSys.h"
#include "worldSys.h"
#include "inputSys.h"
#include "renderSys.h"


class FiniteStateMachine
{
private:
	std::deque<gameState> stateStack;

	int width;
	int height;

	sf::RenderWindow &window;

	RenderSys render;
	InputSys input;
	MenuSys menu;
	WorldSys world;

	bool resolveReturnedState(gameState state);

public:
	FiniteStateMachine(sf::RenderWindow &windowPtr);

	int getWidth();
	int getHeight();
	void resize();

	bool update(float t, const float dt);
	void draw(int frameRate, const float alpha);

	void keyPressed(sf::Keyboard::Key key);
	void keyReleased(sf::Keyboard::Key key);
	void mouseUpdate(sf::Vector2i mousePosition);
};