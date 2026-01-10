#pragma once
#include <SFML/Graphics.hpp>

inline sf::RectangleShape createBar(const sf::Color& color) {
    sf::RectangleShape bar({50,5});
    bar.setFillColor(color);
    return bar;
}
