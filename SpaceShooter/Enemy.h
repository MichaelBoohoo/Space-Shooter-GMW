#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    sf::RectangleShape shape;
    int type;
    int hp;
    float shootTimer = 0.f;

    bool specialDirSet = false;
    float dirX = 0.f;
    float dirY = 0.f;

    Enemy(float x, float y, int t);
    void update(float dt);

    
        sf::FloatRect getHitBox() const {
            return this->shape.getGlobalBounds(); //tworzy ramke wokol gracza/wroga, bedzie dzialac jako hitbox
        }
};
