#pragma once
#include <vector>
#include "PowerUp.h"
#include "Player.h"

class PowerUpManager {
private:
    std::vector<PowerUp>& powerUps;

    bool& doubleShot;
    bool& shield;
    float& shootCooldown;
    float& doubleShotTimer;
    float& fastShotTimer;
    float& shieldTimer;
    const float POWERUP_DURATION;

public:
    PowerUpManager(
        std::vector<PowerUp>& pUps,
        bool& dShot,
        bool& sh,
        float& sc,
        float& dShotT,
        float& fShotT,
        float& shT,
        float duration
    ) : powerUps(pUps), doubleShot(dShot), shield(sh),
        shootCooldown(sc), doubleShotTimer(dShotT),
        fastShotTimer(fShotT), shieldTimer(shT),
        POWERUP_DURATION(duration) {}

    void update(float dt, const sf::RectangleShape& player);
};


