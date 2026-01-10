#pragma once
#include <SFML/Graphics.hpp>

constexpr float PLAYER_SPEED = 300.f;
constexpr float PLAYER_SIZE = 50.f;
constexpr float BULLET_SPEED = 600.f;
constexpr float ENEMY_BULLET_SPEED = 300.f;
constexpr float POWERUP_SPEED = 120.f;
constexpr float POWERUP_DURATION = 5.f;

// Kolory
constexpr sf::Color COLOR_FAST = sf::Color::Green;
constexpr sf::Color COLOR_DOUBLE = sf::Color::Cyan;
constexpr sf::Color COLOR_SHIELD = sf::Color::Magenta;
constexpr sf::Color COLOR_BOSS = sf::Color::White;
constexpr sf::Color COLOR_ENEMY1 = sf::Color::Red;
constexpr sf::Color COLOR_ENEMY2 = sf::Color(180,0,255);
