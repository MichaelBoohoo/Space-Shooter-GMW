#include "WaveManager.h"
#include <cmath>
#include <cstdlib>

//  CONSTRUCTOR 

WaveManager::WaveManager(std::vector<Enemy>& e)
: enemies(e)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    bossHpBack.setSize({ 200.f, 10.f });
    bossHpFront.setSize({ 200.f, 10.f });
    bossHpBack.setFillColor(sf::Color(60, 60, 60));
    bossHpFront.setFillColor(sf::Color::Red);
    bossHpBack.setPosition({ 300.f, 20.f });
    bossHpFront.setPosition({ 300.f, 20.f });

    firstSoloDone = false;
    soloActive = false;
    wavesSinceBoss = 0;
    bossTime = 0.f;

    spawnSolo();
}

//  UPDATE 

void WaveManager::update(float dt, std::vector<EnemyBullet>& enemyBullets)
{
    bool edge = false;

    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& e = enemies[i];
        e.update(dt);

        if (soloActive)
            continue;

        // ruch poziomy prawo-lewo (typ 1 i 2)
        if (e.type == 1 || e.type == 2) {
            e.shape.move(enemyDir * dt);
            if (e.shape.getPosition().x < 20 || e.shape.getPosition().x + 40 > 780)
                edge = true;
        }

        // boss
        if (e.type == 3) {
            bossTime += dt;
            float x = 330.f + std::sin(bossTime * 1.5f) * 200.f;
            e.shape.setPosition({ x, 80.f });

            if (e.shootTimer > 0.6f) {
                float sx = e.shape.getPosition().x + 70;
                float sy = e.shape.getPosition().y + 90;

                enemyBullets.emplace_back(sx - 30, sy, 500.f);
                enemyBullets.emplace_back(sx, sy, 500.f);
                enemyBullets.emplace_back(sx + 30, sy, 500.f);
                e.shootTimer = 0.f;
            }
        }

        // strzelanie wolniejsze dla typu 2
        if (e.type == 2 && rand() % 1200 < 8) {
            enemyBullets.emplace_back(e.shape.getPosition().x + 20,
                e.shape.getPosition().y + 40,
                300.f);
        }
    }

    if (edge)
        enemyDir.x = -enemyDir.x;
}

//  WAVES 

void WaveManager::nextWave()
{
    enemies.clear();

    if (!firstSoloDone) {
        firstSoloDone = true;
        soloActive = false;
        spawnRandomWave();
        return;
    }

    wavesSinceBoss++;

    if (wavesSinceBoss == 3) {
        wavesSinceBoss = 0;
        spawnBoss();
    }
    else {
        spawnRandomWave();
    }
}

void WaveManager::spawnSolo()
{
    enemies.clear();

    Enemy e(380.f, 200.f, 1); // samotny wróg na początku
    e.hp = 3;
    enemies.push_back(e);

    soloActive = true;

    this->soloDialog = true;
}

void WaveManager::spawnRandomWave()
{
    soloActive = false;

    int r = rand() % 4;

    if (r == 0) spawnWaveVShoot();      // 2 wrogowie strzelający wolniej
    if (r == 1) spawnWaveTriangleShoot(); // 3 wrogowie w trójkącie
    if (r == 2) spawnWaveVTriple();     // 8 wrogowie prawo-lewo (zamiast V)
    if (r == 3) spawnWaveHorizontalShoot(); // 8 wrogów prawo-lewo
}

void WaveManager::spawnWaveHorizontalShoot()
{
    enemyDir = { 60.f, 0.f };
    for (int i = 0; i < 8; i++) // zwiększone z 6 do 8
        enemies.emplace_back(100 + i * 80, 100, 2);
}

void WaveManager::spawnWaveTriangleShoot()
{
    // teraz 8 wrogów (trójkąt + dodatkowe rzędy)
    std::vector<sf::Vector2f> pos = {
        {400, 100}, {350, 160}, {450, 160},
        {300, 200}, {400, 200}, {500, 200},
        {250, 240}, {550, 240}
    };
    for (auto& p : pos)
        enemies.emplace_back(p.x, p.y, 2);
}

void WaveManager::spawnWaveVTriple()
{
    // 8 zwykłych wrogów prawo-lewo zamiast litery V
    enemyDir = { 60.f, 0.f };
    for (int i = 0; i < 8; i++) {
        enemies.emplace_back(150.f + i * 70, 40.f, 1);
    }
}

void WaveManager::spawnWaveVShoot()
{
    // Pierwszy statyczny trójkąt
    enemies.emplace_back(300, 120, 2);
    enemies.emplace_back(260, 160, 2);
    enemies.emplace_back(340, 160, 2);

    // Drugi statyczny trójkąt
    enemies.emplace_back(500, 120, 2);
    enemies.emplace_back(460, 160, 2);
    enemies.emplace_back(540, 160, 2);

    // Nie ustawiamy enemyDir → wrogowie są statyczni
}


void WaveManager::spawnBoss()
{
    enemies.clear();
    bossTime = 0.f;
    enemies.emplace_back(330, 80, 3);

    this->bossDialog = true;
}

//  DRAW BOSS HP 

void WaveManager::drawBossHP(sf::RenderTarget& window)
{
    for (auto& e : enemies) {
        if (e.type == 3) {
            bossHpFront.setSize({200.f * (e.hp / 30.f), 10.f});
            window.draw(bossHpBack);
            window.draw(bossHpFront);
        }
    }
}

