#include "inputSys.h"

InputSys::InputSys() : mouseX(0), mouseY(0) {}

void InputSys::keyPressed(sf::Keyboard::Key key) {

	switch (key) {
    case sf::Keyboard::W:
        keys.set(KEY_UP);
        break;
    case sf::Keyboard::A:
        keys.set(KEY_LEFT);
        break;
    case sf::Keyboard::S:
        keys.set(KEY_DOWN);
        break;
    case sf::Keyboard::D:
        keys.set(KEY_RIGHT);
        break;
	case sf::Keyboard::Q:
		keys.set(KEY_ASC);
		break;
	case sf::Keyboard::E:
		keys.set(KEY_DSC);
		break;
	case sf::Keyboard::Space:
		keys.set(KEY_FIRE);
		break;
	case sf::Keyboard::Escape:
		keys.set(KEY_EXIT);
		break;
    }
}

void InputSys::keyReleased(sf::Keyboard::Key key) {

	switch (key) {
    case sf::Keyboard::W:
        keys.reset(KEY_UP);
        break;
    case sf::Keyboard::A:
        keys.reset(KEY_LEFT);
        break;
    case sf::Keyboard::S:
        keys.reset(KEY_DOWN);
        break;
    case sf::Keyboard::D:
        keys.reset(KEY_RIGHT);
        break;
	case sf::Keyboard::Q:
		keys.reset(KEY_ASC);
		break;
	case sf::Keyboard::E:
		keys.reset(KEY_DSC);
		break;
	case sf::Keyboard::Space:
		keys.reset(KEY_FIRE);
		break;
	case sf::Keyboard::Escape:
		keys.reset(KEY_EXIT);
		break;
    }
}

bool InputSys::isKeyPressed(KeyState key) { return keys[key]; }

void InputSys::mouseUpdate(sf::Vector2i mousePosition) {

	mouseX = mousePosition.x;
	mouseY = mousePosition.y;

}

InputResponse InputSys::update() {

	return InputResponse(keys, mouseX, mouseY);

}