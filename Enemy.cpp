#include "Enemy.h"

Enemy::Enemy(float x, float y, int t) : type(t) {
    if (type == 1) {
        shape.setSize({40, 40});
        shape.setFillColor(sf::Color::Red);
        hp = 1;
    } else if (type == 2) {
        shape.setSize({40, 40});
        shape.setFillColor(sf::Color(180, 0, 255));
        hp = 2;
    } else { // boss
        shape.setSize({140, 90});
        shape.setFillColor(sf::Color::White);
        hp = 30;
    }
    shape.setPosition({x, y});
}

void Enemy::update(float dt) {
    shootTimer += dt;
}
