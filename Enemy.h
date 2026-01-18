#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    sf::RectangleShape shape;
    int type;
    int hp;
    float shootTimer = 0.f;

    Enemy(float x, float y, int t);
    void update(float dt);
};
