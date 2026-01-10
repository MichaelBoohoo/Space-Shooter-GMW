#pragma once
#include <SFML/Graphics.hpp>

enum PowerUpType {
    SHIELD = 1,
    FAST_SHOT = 2,
    DOUBLE_SHOT = 3
};

class PowerUp {
public:
    sf::CircleShape shape;
    PowerUpType type;
    float speed = 120.f;

    PowerUp(float x, float y, PowerUpType t);
    void update(float dt);

    sf::Color getColor() const;
};
