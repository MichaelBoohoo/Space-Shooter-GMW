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
    sf::Vector2f enemyDir = { 60.f, 0.f };

    // Timer do ruchu bossa
    int wavesSinceBoss = 0;
    float bossTime = 0.f;

    

    // Numer aktualnej fali
    int wave;

    // Pasek HP bossa
    sf::RectangleShape bossHpBack;
    sf::RectangleShape bossHpFront;

    bool firstSoloDone = false;
    bool soloActive = false;

public:
    bool bossDialog = false;
    bool soloDialog = false;
    // Konstruktor – dostaje referencję do wrogów z Game
    WaveManager(std::vector<Enemy>& e);

    // Aktualizacja ruchu, strzelania i zachowań wrogów
    void update(float dt, std::vector<EnemyBullet>& enemyBullets);

    // Przejście do kolejnej fali
    void nextWave();

    // Rysowanie paska HP bossa (jeśli jest aktywny)
    void drawBossHP(sf::RenderTarget& window);

    // Funkcje spawnu
    void spawnSolo();
    void spawnRandomWave();
    void spawnWaveHorizontalShoot();
    void spawnWaveTriangleShoot();
    void spawnWaveVTriple();
    void spawnWaveVShoot();
    void spawnBoss();
};

