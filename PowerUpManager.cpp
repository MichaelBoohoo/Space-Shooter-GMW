#include "PowerUpManager.h"
#include "Collision.h"

void PowerUpManager::update(float dt, const sf::RectangleShape& player) {

    for(auto& p : powerUps) p.update(dt);

    if(doubleShot) {
        doubleShotTimer -= dt;
        if(doubleShotTimer <= 0.f) doubleShot = false;
    }
    if(shootCooldown < 0.4f) {
        fastShotTimer -= dt;
        if(fastShotTimer <= 0.f) shootCooldown = 0.4f;
    }
    if(shield) {
        shieldTimer -= dt;
        if(shieldTimer <= 0.f) shield = false;
    }
}


