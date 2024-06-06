#include "Button.h"
#include "Scene.h"

int main() {
	const int windowWidth = 900;
	const int windowHeight = 650;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Window");
	window.setFramerateLimit(60);

	Scene scene(&window);

	const int buttonWidth = 100;
	const int buttonHeight = 50;
	Button button_start(&window, 0, 500, buttonWidth, buttonHeight, "Find", sf::Color(51, 51, 255));
	Button button_rand(&window, 0, 550, buttonWidth, buttonHeight, "Rand", sf::Color(240, 120, 0));
	Button button_clear(&window, 0, 600, buttonWidth, buttonHeight, "Clear", sf::Color(204, 0, 204));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				int mouseX = event.mouseButton.x;
				int mouseY = event.mouseButton.y;

				if (event.mouseButton.button == sf::Mouse::Left) {
					button_start.handleClick(mouseX, mouseY, [&scene]() {
						scene.runDijkstra();
					});

					button_rand.handleClick(mouseX, mouseY, [&scene]() {
						scene.setRandomStartEnd();
					});

					button_clear.handleClick(mouseX, mouseY, [&scene]() {
						scene.clearGrid();
					});

					if (event.type == sf::Event::MouseButtonReleased) {
						scene.handleMouseRelease();
					}
				}

				if (event.mouseButton.button == sf::Mouse::Right) {
					scene.handleRightClick(mouseX, mouseY);
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				scene.updateMouseDragState();
			}
		}

		window.clear();
		scene.draw();
		button_start.draw();
		button_rand.draw();
		button_clear.draw();
		window.display();
	}

	return 0;
}