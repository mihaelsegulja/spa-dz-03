#include "Button.h"
#include "Scene.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(900,600), "Window");
	window.setFramerateLimit(60);

	Scene scene(&window);

	Button button_start(&window, 0, 500, 100, 50, "Find", sf::Color(51, 51, 255));
	Button button_rand(&window, 0, 550, 100, 50, "Rand", sf::Color(240, 120, 0));

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
						//std::cout << "Run clicked!" << std::endl;
						scene.runDijkstra();
					});

					button_rand.handleClick(mouseX, mouseY, [&scene]() {
						//std::cout << "Random clicked!" << std::endl;
						scene.setRandomStartEnd();
					});

					scene.handleLeftClick(mouseX, mouseY);
				}

				if (event.mouseButton.button == sf::Mouse::Right) {
					scene.handleRightClick(mouseX, mouseY);
				}
			}
		}

		window.clear();
		scene.draw();
		button_start.draw();
		button_rand.draw();
		window.display();
	}

	return 0;
}