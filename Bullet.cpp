#include "Bullet.h"

// ================= PLAYER BULLET =================
Bullet::Bullet(float x, float y) {
    shape.setSize({5.f, 15.f});
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition({x, y});
}

void Bullet::update(float dt) {
    shape.move({0.f, -speed * dt});
}

// ================= ENEMY BULLET =================
EnemyBullet::EnemyBullet(float x, float y, float s) : speed(s) {
    shape.setSize({5.f, 15.f});
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({x, y});
}

void EnemyBullet::update(float dt) {
    shape.move({0.f, speed * dt});
}
