#include "Enemy.h"

Enemy::Enemy(float x, float y, int t) : type(t)
{
    if (type == 1) { // standard
        shape.setSize({ 40, 40 });
        shape.setFillColor(sf::Color::Red);
        hp = 1;
    }
    else if (type == 2) { // strzelaj¹cy i ruszaj¹cy siê
        shape.setSize({ 40, 40 });
        shape.setFillColor(sf::Color(180, 0, 255));
        hp = 2;
    }
    else if (type == 3) { // boss
        shape.setSize({ 140, 90 });
        shape.setFillColor(sf::Color::White);
        hp = 30;
    }
    else if (type == 4) { // samotny stoj¹cy
        shape.setSize({ 40, 40 });
        shape.setFillColor(sf::Color::Yellow);
        hp = 1;
    }
    else if (type == 6) { // diagonal V
        shape.setSize({ 40, 40 });
        shape.setFillColor(sf::Color::Cyan);
        hp = 2;
    }

    shape.setPosition({ x, y });
}


void Enemy::update(float dt) {
    shootTimer += dt;
}
