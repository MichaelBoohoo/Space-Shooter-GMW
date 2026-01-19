#include "Enemy.h"
#include <iostream>

// Definicja zmiennych statycznych
sf::Texture Enemy::t1;
sf::Texture Enemy::t2;
sf::Texture Enemy::t3;
bool Enemy::texturesLoaded = false;

void Enemy::loadTextures() {
    if (!texturesLoaded) {
        // Próba załadowania tekstur z plików
        // Wypisze komunikat w konsoli tylko jeśli pliku brakuje
        if (!t1.loadFromFile("zly1.png")) std::cerr << "Blad: brak zly1.png\n";
        if (!t2.loadFromFile("zly2.png")) std::cerr << "Blad: brak zly2.png\n";
        if (!t3.loadFromFile("zly3.png")) std::cerr << "Blad: brak zly3.png\n";

        texturesLoaded = true;
    }
}

Enemy::Enemy(float x, float y, int t) : type(t) {
    loadTextures(); // Upewnij się, że tekstury są w pamięci

    // Ustawienia domyślne pozycji
    shape.setPosition({ x, y });

    if (type == 1) {
        // Przeciwnik zwykły
        shape.setSize({ 40, 40 });
        hp = 1;

        if (t1.getSize().x > 0) { // Jeśli tekstura załadowana
            shape.setTexture(&t1);
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(sf::Color::Red); // Fallback (brak pliku)
        }

    }
    else if (type == 2) {
        // Przeciwnik strzelający
        shape.setSize({ 40, 40 });
        hp = 2;

        if (t2.getSize().x > 0) {
            shape.setTexture(&t2);
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(sf::Color(180, 0, 255)); // Fallback
        }

    }
    else {
        // BOSS (type 3)
        shape.setSize({ 140, 90 });
        hp = 30;

        if (t3.getSize().x > 0) {
            shape.setTexture(&t3);
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(sf::Color::White); // Fallback
        }
    }
}

void Enemy::update(float dt) {
    shootTimer += dt;
}