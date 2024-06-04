#include "Button.h"

Button::Button(sf::RenderWindow* window, float x, float y, float width, float height, const sf::String& text, const sf::Color& color) {
    this->window = window;
    
    buttonShape.setPosition(x, y);
    buttonShape.setSize(sf::Vector2f(width, height));
    buttonShape.setFillColor(color);

    if (!font.loadFromFile("BigBlueTermPlusNerdFont-Regular.ttf")) {
        std::cout << "Can't load font!" << std::endl;
    }

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(
        x + (width - buttonText.getGlobalBounds().width) / 2,
        y + (height - buttonText.getGlobalBounds().height) / 2
    );
}

void Button::draw() {
    window->draw(buttonShape);
    window->draw(buttonText);
}

void Button::handleClick(int mouseX, int mouseY, std::function<void()> onClick) {
    if (buttonShape.getGlobalBounds().contains(mouseX, mouseY)) {
        onClick();
    }
}
