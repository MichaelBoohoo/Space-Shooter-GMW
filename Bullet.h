#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    sf::RectangleShape shape;
    float speed = 600.f;

    Bullet(float x, float y);
    void update(float dt);
};

class EnemyBullet {
public:
    sf::RectangleShape shape;
    float speed;

    EnemyBullet(float x, float y, float s);
    void update(float dt);
};
