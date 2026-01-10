#pragma once
#include <SFML/Graphics.hpp>

inline bool intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}
