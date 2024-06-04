#pragma once
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>

class Button {
public:
    Button(sf::RenderWindow* window, float x, float y, float width, float height, const sf::String& text, const sf::Color& color);
    void draw();
    void handleClick(int mouseX, int mouseY, std::function<void()> onClick);

private:
    sf::RenderWindow* window;
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
};