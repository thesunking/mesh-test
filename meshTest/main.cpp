#include "stateManager.h"

int main(int argc, char** argv) {

	srand( (unsigned int)time(NULL) );

	sf::RenderWindow window(sf::VideoMode(900, 900), "Mesh Test");
	window.setVerticalSyncEnabled(true);

	sf::ContextSettings settings = window.getSettings();
	printf("OpenGL version: %d.%d\n", settings.majorVersion, settings.minorVersion);

	sf::Vector2u init_size = window.getSize();
	sf::Mouse::setPosition( sf::Vector2i( (int)(init_size.x * 0.5), (int)(init_size.y * 0.5) ), window );

	FiniteStateMachine fsm(window);

	sf::Clock clock;
	float t = 0.0f;
	const float dt = 0.05f;
	float currentTime = (clock.getElapsedTime()).asSeconds();
	float accumulator = 0.0f;
	int frameRate = 0;

    while (window.isOpen()) {

		float newTime = (clock.getElapsedTime()).asSeconds();
        float frameTime = newTime - currentTime;
		if ( frameTime > 0.25f ) {frameTime = 0.25f;}		// note: max frame time to avoid spiral of death
        currentTime = newTime;
        accumulator += frameTime;

        while ( accumulator >= dt ) {

			sf::Event event;
			while (window.pollEvent(event)) {
			// check the type of the event...
				switch (event.type) {

					// window closed
					case sf::Event::Closed:
						window.close();
						return 0;

					// key pressed
					case sf::Event::KeyPressed:
						fsm.keyPressed(event.key.code);
						break;

					// key released
					case sf::Event::KeyReleased:
						fsm.keyReleased(event.key.code);
						break;

					default:
						break;
				}
			}

			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			mousePosition.x -= 450;
			mousePosition.y -= 450;
			mousePosition.y  = -mousePosition.y;
			fsm.mouseUpdate(mousePosition);
			
			//printf("main:update\n");

			if (!(fsm.update(t, dt))) {
				window.close();
				return 0;
			}

            t += dt;
            accumulator -= dt;
        }
		
        const float alpha = accumulator / dt;
		frameRate = (int)(0.25f * (1 / frameTime) + 0.75f * (float)frameRate);
		//printf("main:draw\n");
		fsm.draw(frameRate, alpha);
        window.display();

    }

    return 0;
}