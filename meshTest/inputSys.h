#pragma once
#include <SFML\Graphics.hpp>
#include <deque>
#include "inputResponse.h"

class InputSys {
	
public:
	keyBuffer keys;
	int mouseX;
	int mouseY;
	
	InputSys();
	//~InputSys();
	
	void keyPressed(sf::Keyboard::Key key);
	void keyReleased(sf::Keyboard::Key key);
	bool isKeyPressed(KeyState key);
	
	void mouseUpdate(sf::Vector2i mousePosition);

	InputResponse update();
	
};