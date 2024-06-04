#include "Button.h"
#include "Scene.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(800,500), "Window");
	window.setFramerateLimit(60);

	Scene scene(&window);

	Button button_start(&window, 0, 400, 100, 50, "Find", sf::Color::Blue);
	Button button_rand(&window, 0, 450, 100, 50, "Random", sf::Color::Magenta);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					int mouseX = event.mouseButton.x;
					int mouseY = event.mouseButton.y;

					button_start.handleClick(mouseX, mouseY, [&scene]() {
						//std::cout << "Run clicked!" << std::endl;
						scene.runDijkstra();
					});

					button_rand.handleClick(mouseX, mouseY, [&scene]() {
						//std::cout << "Random clicked!" << std::endl;
						scene.setRandomStartEnd();
					});

					scene.handleClick(mouseX, mouseY);
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					//TODO: add function to handle setting start/end block
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