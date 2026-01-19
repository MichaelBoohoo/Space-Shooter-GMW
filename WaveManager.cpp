#include "WaveManager.h"
#include <cmath>
#include <cstdlib>

// ---------------- CONSTRUCTOR ----------------

WaveManager::WaveManager(std::vector<Enemy>& e)
    : enemies(e)
{
    bossHpBack.setSize({ 200.f, 10.f });
    bossHpFront.setSize({ 200.f, 10.f });
    bossHpBack.setFillColor(sf::Color(60, 60, 60));
    bossHpFront.setFillColor(sf::Color::Red);
    bossHpBack.setPosition({ 300, 20 });
    bossHpFront.setPosition({ 300, 20 });

    spawnWave1();
}

// ---------------- UPDATE ----------------

void WaveManager::update(float dt, std::vector<EnemyBullet>& enemyBullets)
{
    // Logika ruchu dla zwykłych fal (lewo-prawo)
    if (wave == 0 || wave == 1 || wave == 2) {
        bool edge = false;
        for (auto& e : enemies) {
            e.shape.move(enemyDir * dt);
            if (e.shape.getPosition().x < 20 ||
                e.shape.getPosition().x + 40 > 780)
                edge = true;
        }
        if (edge) enemyDir.x = -enemyDir.x;
    }

    // Logika ruchu Bossa (sinusoida)
    if (wave == 3 && !enemies.empty()) {
        bossTime += dt;
        float x = 330 + std::sin(bossTime * 1.5f) * 200.f;
        // Boss jest zawsze pierwszy w wektorze w tej fali
        enemies[0].shape.setPosition({ x, 80 });
    }

    // Aktualizacja każdego wroga i strzelanie
    for (auto& e : enemies) {
        e.update(dt);

        // Strzelanie fioletowych (Type 2)
        if (e.type == 2 && rand() % 1000 < 10)
            enemyBullets.emplace_back(
                e.shape.getPosition().x + 20,
                e.shape.getPosition().y + 40,
                300.f
            );

        // Strzelanie Bossa (Type 3)
        if (e.type == 3 && e.shootTimer > 0.6f) {
            float x = e.shape.getPosition().x + 70;
            float y = e.shape.getPosition().y + 90;
            // Trzy pociski na raz
            enemyBullets.emplace_back(x - 30, y, 500.f);
            enemyBullets.emplace_back(x, y, 500.f);
            enemyBullets.emplace_back(x + 30, y, 500.f);
            e.shootTimer = 0.f;
        }
    }
}

// ---------------- WAVES ----------------

void WaveManager::nextWave()
{
    wave = (wave + 1) % 4;

    if (wave == 0) spawnWave1();
    if (wave == 1) spawnWave2();
    if (wave == 2) spawnWave3();
    if (wave == 3) spawnBoss();
}

void WaveManager::spawnWave1()
{
    enemies.clear();
    enemyDir = { 80.f, 0.f };

    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 6; c++)
            enemies.emplace_back(80 + c * 110, 60 + r * 60, 1);
}

void WaveManager::spawnWave2()
{
    enemies.clear();
    enemyDir = { 100.f, 0.f }; // Trochę szybciej dla 2 fali
    std::vector<sf::Vector2f> pos = {
        {400,100},{350,150},{450,150},
        {300,200},{400,200},{500,200}
    };
    for (auto& p : pos)
        enemies.emplace_back(p.x, p.y, 2);
}

void WaveManager::spawnWave3()
{
    enemies.clear();
    enemyDir = { 120.f, 0.f };
    std::vector<sf::Vector2f> pos = {
        {300,100},{500,100},
        {330,150},{470,150},
        {360,200},{440,200},
        {400,250}
    };
    for (auto& p : pos)
        enemies.emplace_back(p.x, p.y, 2);
}

void WaveManager::spawnBoss()
{
    enemies.clear();
    bossTime = 0.f;
    enemies.emplace_back(330, 80, 3); // Type 3 = Boss
}

// ---------------- DRAW BOSS HP ----------------

void WaveManager::drawBossHP(sf::RenderTarget& window)
{
    for (auto& e : enemies) {
        if (e.type == 3) {
            // Skalowanie paska życia
            float percent = (float)e.hp / 30.f;
            if (percent < 0) percent = 0;
            bossHpFront.setSize({ 200.f * percent, 10.f });

            window.draw(bossHpBack);
            window.draw(bossHpFront);
        }
    }
}