#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enemy.h"
#include "Bullet.h"

// Zarządza falami przeciwników (klasyczne fale + boss)
class WaveManager {
private:
    // Referencja do wektora wrogów z gry
    std::vector<Enemy>& enemies;

    // Kierunek ruchu całej fali
    sf::Vector2f enemyDir;

    // Timer do ruchu bossa
    float bossTime;

    // Numer aktualnej fali
    int wave;

    // Pasek HP bossa
    sf::RectangleShape bossHpBack;
    sf::RectangleShape bossHpFront;

public:
    // Konstruktor – dostaje referencję do wrogów z Game
    WaveManager(std::vector<Enemy>& e);

    // Aktualizacja ruchu, strzelania i zachowań wrogów
    void update(float dt, std::vector<EnemyBullet>& enemyBullets);

    // Przejście do kolejnej fali
    void nextWave();

    // Rysowanie paska HP bossa (jeśli jest aktywny)
    void drawBossHP(sf::RenderTarget& window);

    // Spawny fal
    void spawnWave1();
    void spawnWave2();
    void spawnWave3();
    void spawnBoss();
};

