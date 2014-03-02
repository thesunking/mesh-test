#include <Windows.h>
#include <memory>

#include "stateManager.h"

static size_t GetStackUsage();

int main(int argc, char** argv) {

	printf("Stack: %Iu\n", GetStackUsage());

	srand( (unsigned int)time(NULL) );

	sf::RenderWindow window(sf::VideoMode(900, 900), "Mesh Test");
	window.setVerticalSyncEnabled(true);

	sf::ContextSettings settings = window.getSettings();
	printf("OpenGL version: %d.%d\n", settings.majorVersion, settings.minorVersion);

	sf::Vector2u init_size = window.getSize();
	sf::Mouse::setPosition( sf::Vector2i( (int)(init_size.x * 0.5), (int)(init_size.y * 0.5) ), window );

	auto pFSM = std::make_unique<FiniteStateMachine>(window);

	printf("Stack: %Iu\n", GetStackUsage());

	sf::Clock clock;
	float t = 0.0f;
	const float dt = 0.01f;
	float currentTime = (clock.getElapsedTime()).asSeconds() - 1.5f * dt; //force update call to occur before draw call
	float accumulator = 0.0f;
	int frameRate = 0;

	float newTime;
	float frameTime;

    while (window.isOpen()) {

		newTime = (clock.getElapsedTime()).asSeconds();
        frameTime = newTime - currentTime;

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
						pFSM->keyPressed(event.key.code);
						break;

					// key released
					case sf::Event::KeyReleased:
						pFSM->keyReleased(event.key.code);
						break;

					default:
						break;
				}
			}

			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			mousePosition.x -= 450;
			mousePosition.y -= 450;
			mousePosition.y  = -mousePosition.y;
			pFSM->mouseUpdate(mousePosition);
			
			//printf("main:update\n");

			if (!(pFSM->update(t, dt))) {
				window.close();
				return 0;
			}

            t += dt;
            accumulator -= dt;
        }
		
        const float alpha = accumulator / dt;
		frameRate = (int)(0.25f * (1 / frameTime) + 0.75f * (float)frameRate);
		//printf("main:draw\n");
		pFSM->draw(frameRate, alpha);
        window.display();

    }

    return 0;
}

static size_t GetStackUsage() {
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(&mbi, &mbi, sizeof(mbi));
	// now mbi.AllocationBase = reserved stack memory base address

	VirtualQuery(mbi.AllocationBase, &mbi, sizeof(mbi));
	// now (mbi.BaseAddress, mbi.RegionSize) describe reserved (uncommitted) portion of the stack
	// skip it

	VirtualQuery((char*)mbi.BaseAddress + mbi.RegionSize, &mbi, sizeof(mbi));
	// now (mbi.BaseAddress, mbi.RegionSize) describe the guard page
	// skip it

	VirtualQuery((char*)mbi.BaseAddress + mbi.RegionSize, &mbi, sizeof(mbi));
	// now (mbi.BaseAddress, mbi.RegionSize) describe the committed (i.e. accessed) portion of the stack

	return mbi.RegionSize;
}