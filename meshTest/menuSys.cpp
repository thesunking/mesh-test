#include "menuSys.h"

gameState MenuSys::update(InputResponse data) {

	gameState returnState = MENU;

	if (data.keys[KEY_EXIT]) {returnState = RETURN;}
	if (data.keys[KEY_FIRE]) {returnState = GAME;}

	return returnState;

}

void MenuSys::draw(RenderSys &render) {}