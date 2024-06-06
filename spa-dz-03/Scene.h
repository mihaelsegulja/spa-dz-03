#pragma once
#include <ctime>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>

class Scene
{
private:
	sf::RenderWindow* window;
	std::vector<sf::RectangleShape> grid;
	sf::Text note;
	sf::Font font;
	sf::RectangleShape startBlock;
	sf::RectangleShape endBlock;
	int startIndex;
	int endIndex;
	std::vector<std::vector<int>> adjList;
	void createAdjList();
	void visualizePath(const std::vector<int>& previous);
	bool isDragging = false;
	
public:
	Scene(sf::RenderWindow* window);
	void handleMouseDrag(int mouseX, int mouseY);
	void handleRightClick(int mouseX, int mouseY);
	void draw();
	void setRandomStartEnd();
	void runDijkstra();
	void handleMouseRelease();
	void updateMouseDragState();
	void clearGrid();
};

