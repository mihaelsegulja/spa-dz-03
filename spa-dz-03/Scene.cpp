#include "Scene.h"

const float INF = std::numeric_limits<float>::infinity();
const int gridWidth = 45;
const int gridHeight = 25;
const int gridSize = 20;
bool settingStartBlock = true;
std::stringstream ss; 

Scene::Scene(sf::RenderWindow* window) {
    this->window = window;

    //grid
    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            sf::RectangleShape cell(sf::Vector2f(gridSize - 1, gridSize - 1));
            cell.setFillColor(sf::Color::White);
            cell.setPosition(col * gridSize, row * gridSize);
            grid.push_back(cell);
        }
    }

    //note
    if (!font.loadFromFile("BigBlueTermPlusNerdFont-Regular.ttf")) {
        std::cout << "Can't load font!" << std::endl;
    }
    ss << "Set obstacles by pressing/holding left click." 
        << "\nSet start/end positions with right click.\n"
        << "\nPress \"Rand\" to randomize start and end positions."
        << "\nPress \"Find\" to find path from start to end."
        << "\nPress \"Clear\" to clear obstacles.";
    note.setFont(font);
    note.setString(ss.str());
    note.setCharacterSize(18);
    note.setFillColor(sf::Color::White);
    note.setPosition(120, 510);

    //start & end blocks
    srand(time(nullptr));
    startIndex = -1;
    endIndex = -1;
    startBlock.setSize(sf::Vector2f(gridSize - 1, gridSize - 1));
    startBlock.setFillColor(sf::Color::Red);
    endBlock.setSize(sf::Vector2f(gridSize - 1, gridSize - 1));
    endBlock.setFillColor(sf::Color::Green);


    createAdjList();
}

void Scene::handleMouseDrag(int mouseX, int mouseY) {
    for (int i = 0; i < grid.size(); i++) {
        if (grid[i].getGlobalBounds().contains(mouseX, mouseY)) {
            if (grid[i].getFillColor() == sf::Color::White || grid[i].getFillColor() == sf::Color::Cyan) {
                grid[i].setFillColor(sf::Color::Black);
            }
            break;
        }
    }
}

void Scene::handleMouseRelease() {
    isDragging = false;
}

void Scene::updateMouseDragState() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isDragging = true;
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        handleMouseDrag(mousePos.x, mousePos.y);
    }
}

void Scene::clearGrid() {
    for (int i = 0; i < grid.size(); i++) {
        if (grid[i].getFillColor() == sf::Color::Black)
            grid[i].setFillColor(sf::Color::White);
    }
}

void Scene::handleRightClick(int mouseX, int mouseY) {
    for (int i = 0; i < grid.size(); i++) {
        if (grid[i].getGlobalBounds().contains(mouseX, mouseY)) {
            if (grid[i].getFillColor() == sf::Color::White || grid[i].getFillColor() == sf::Color::Cyan) {
                if (settingStartBlock) {
                    startBlock.setPosition(grid[i].getPosition());
                    startIndex = i;
                    settingStartBlock = false;
                }
                else {
                    endBlock.setPosition(grid[i].getPosition());
                    endIndex = i;
                    settingStartBlock = true;
                }
            }
            break;
        }
    }
}

void Scene::draw() {
    for (const auto& cell : grid)
        window->draw(cell);

    if (startIndex != -1)
        window->draw(startBlock);

    if (endIndex != -1)
        window->draw(endBlock);

    window->draw(note);
}

void Scene::createAdjList() {
    adjList.resize(gridHeight * gridWidth);

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            int index = row * gridWidth + col;

            if (row > 0) 
                adjList[index].push_back(index - gridWidth); //up
            if (row < gridHeight - 1) 
                adjList[index].push_back(index + gridWidth); //down
            if (col > 0) 
                adjList[index].push_back(index - 1); //left
            if (col < gridWidth - 1) 
                adjList[index].push_back(index + 1); //right
        }
    }
}

void Scene::setRandomStartEnd() {
    if (!grid.empty()) {
        startIndex = std::rand() % grid.size();
        endIndex = std::rand() % grid.size();

        //end & start can't be the same, and can't be an obstacle
        while (endIndex == startIndex || grid[startIndex].getFillColor() == sf::Color::Black || grid[endIndex].getFillColor() == sf::Color::Black) {
            startIndex = std::rand() % grid.size();
            endIndex = std::rand() % grid.size();
        }

        startBlock.setPosition(grid[startIndex].getPosition());
        endBlock.setPosition(grid[endIndex].getPosition());
    }
}

void Scene::runDijkstra() {
    if (startIndex == -1 || endIndex == -1) 
        return;

    std::vector<float> minDist(grid.size(), INF);
    std::vector<int> previous(grid.size(), -1);
    minDist[startIndex] = 0;

    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;
    pq.push({ 0, startIndex });

    while (!pq.empty()) {
        float dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > minDist[u]) 
            continue;

        for (int v : adjList[u]) {
            if (grid[v].getFillColor() == sf::Color::Black)
                continue;
            float weight = 1.0f;
            float distanceThroughU = dist + weight;
            if (distanceThroughU < minDist[v]) {
                minDist[v] = distanceThroughU;
                previous[v] = u;
                pq.push({ distanceThroughU, v });
                grid[v].setFillColor(sf::Color::White);
            }
        }
    }

    visualizePath(previous);
}

void Scene::visualizePath(const std::vector<int>& previous) {
    int v = endIndex;
    sf::Clock clock;
    const float delay = 0.05f;

    while (v != -1) {
        grid[v].setFillColor(sf::Color::Cyan);

        window->clear(sf::Color::Black);
        draw();
        window->display();
        
        while (clock.getElapsedTime().asSeconds() < delay) {
            // poll events to keep the window responsive
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window->close();
            }
        }
        clock.restart();
        
        v = previous[v];
    }
}
