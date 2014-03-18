#include "stateManager.h"
#include <bitset>

FiniteStateMachine::FiniteStateMachine(sf::RenderWindow &windowPtr) : width(900), height(900), window(windowPtr), render(window), world(render)
{
	stateStack.push_front(MENU);
}

int FiniteStateMachine::getWidth()
{
	return width;
}

int FiniteStateMachine::getHeight()
{
	return height;
}

void FiniteStateMachine::resize()
{
	sf::Vector2u size = window.getSize();
	width = size.x;
	height = size.y;
}

bool FiniteStateMachine::update(float t, const float dt)
{
	gameState returnedState = RETURN;
	gameState currentState = stateStack.front();

	InputResponse inputRes = input.update();

	if (inputRes.keys[KEY_EXIT])
	{
		return resolveReturnedState(RETURN);
	}

	switch (currentState)
	{
	case GAME:
		returnedState = world.update(inputRes, window, t, dt);
		break;
	case MENU:
		returnedState = menu.update(inputRes);
		break;
	}

	return resolveReturnedState(returnedState);
}

void FiniteStateMachine::draw(int frameRate, const float alpha)
{
	render.prepare();

	switch (stateStack.front())
	{
	case GAME:
		world.draw(render);
		break;
	case MENU:
		menu.draw(render);
		break;
	}

	//render.drawNumber(glm::vec3(3.0f, 3.0f, 0.0f), frameRate);
}

bool FiniteStateMachine::resolveReturnedState(gameState state)
{
	if (state == RETURN)
	{
		stateStack.pop_front();
		if (stateStack.size() == 0)
		{
			window.close();
			return false;
		}
		input.keys.reset(KEY_EXIT);
	}
	else if (state != stateStack.front())
	{
		stateStack.push_front(state);
	}

	if (stateStack.front() == GAME)
	{
		window.setMouseCursorVisible(false);
	}
	else
	{
		window.setMouseCursorVisible(true);
	}

	return true;
}

void FiniteStateMachine::keyPressed(sf::Keyboard::Key key)
{
	input.keyPressed(key);
}

void FiniteStateMachine::keyReleased(sf::Keyboard::Key key)
{
	input.keyReleased(key);
}

void FiniteStateMachine::mouseUpdate(sf::Vector2i mousePosition)
{
	input.mouseUpdate(mousePosition);
}