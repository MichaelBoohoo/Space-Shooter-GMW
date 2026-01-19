#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, PowerUpType t) : type(t) {
    shape.setRadius(10.f);
    shape.setPosition({x, y});
    shape.setFillColor( getColor());
}

void PowerUp::update(float dt) {
    shape.move({0.f, speed * dt});
}

sf::Color PowerUp::getColor() const {
    switch(type) {
        case SHIELD:    return sf::Color::Green;
        case FAST_SHOT: return sf::Color::Cyan;
        case DOUBLE_SHOT:return sf::Color::Magenta;
        default:        return sf::Color::White;
    }
}
