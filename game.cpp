#include "game.h"
#include <cstdlib>  // dla rand()

// ============================
// FUNKCJE PRYWATNE
// ============================

void game::initZmienne()
{
    this->okno = nullptr;
    // Można tu też inicjalizować inne zmienne Galagi jeśli trzeba
}

void game::initOkno()
{
    this->okno = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Shooter");
    this->okno->setFramerateLimit(60);
}

void game::initGracz()
{
    this->gracz = new Player(375.f, 520.f);
}

void game::initManagers()
{
    // Menedżery fal i power-upów
    waveManager = new WaveManager(enemies);
    powerUpManager = new PowerUpManager(
        powerUps,
        doubleShot,
        shield,
        shootCooldown,
        doubleShotTimer,
        fastShotTimer,
        shieldTimer,
        POWERUP_DURATION
    );
}

// ============================
// HUD
// ============================

void game::initHUD()
{
    if (!font.loadFromFile("arial.ttf")) {
        // obsługa błędu ładowania fontu
    }

    powerUpText.setFont(font);
    powerUpText.setCharacterSize(14);
    powerUpText.setFillColor(sf::Color::White);

    powerUpBar.setSize({50.f, 5.f});
    powerUpBar.setFillColor(sf::Color::Green);
}

// ============================
// KONSTRUKTOR I DESTRUKTOR
// ============================

game::game()
{
    this->initZmienne();
    this->initOkno();
    this->initGracz();
    this->initManagers();
    this->initHUD();  // inicjalizacja HUD (paski i font)
}

game::~game()
{
    delete this->okno;
    delete this->gracz;
    delete waveManager;
    delete powerUpManager;
}

// ============================
// DOSTĘP
// ============================

const bool game::OknoOtwarte() const
{
    return this->okno->isOpen();
}

// ============================
// OBSŁUGA EVENTÓW
// ============================

void game::updateEvent()
{
    while (this->okno->pollEvent(this->event))
    {
        if (this->event.type == sf::Event::Closed)
            this->okno->close();
        else if (this->event.key.code == sf::Keyboard::Escape)
            this->okno->close();
    }
}

// ============================
// UPDATE
// ============================

void game::update()
{
    float dt = 1.f / 60.f; // przybliżony delta time

    this->updateEvent();

    // update gracza
    this->gracz->update_Player();

    // ------------------------------
    // STRZAŁ GRACZA
    // ------------------------------
    shootTimer += dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= shootCooldown)
    {
        if (!doubleShot)
            bullets.emplace_back(gracz->getX() + 22, gracz->getY());
        else
        {
            bullets.emplace_back(gracz->getX() + 5, gracz->getY());
            bullets.emplace_back(gracz->getX() + 40, gracz->getY());
        }
        shootTimer = 0.f;
    }

    // ------------------------------
    // UPDATE OBIEKTÓW
    // ------------------------------
    for (auto& b : bullets) b.update(dt);
    for (auto& eb : enemyBullets) eb.update(dt);
    for (auto& p : powerUps) p.update(dt);

    // update menedżerów
    waveManager->update(dt, enemyBullets);
    powerUpManager->update(dt, gracz->getShape());

    // ------------------------------
    // TIMER POWER-UP
    // ------------------------------
    if (doubleShot)
    {
        doubleShotTimer -= dt;
        if (doubleShotTimer <= 0.f) doubleShot = false;
    }
    if (shootCooldown < 0.4f)
    {
        fastShotTimer -= dt;
        if (fastShotTimer <= 0.f) shootCooldown = 0.4f;
    }
    if (shield)
    {
        shieldTimer -= dt;
        if (shieldTimer <= 0.f) shield = false;
    }

    // ------------------------------
    // KOLIZJE
    // ------------------------------
    
   // Bullet -> Enemy
for (size_t i = 0; i < bullets.size(); i++)
{
    for (size_t j = 0; j < enemies.size(); j++)
    {
        if (intersects(bullets[i].shape.getGlobalBounds(),
                       enemies[j].shape.getGlobalBounds()))
        {
            enemies[j].hp--;
            bullets.erase(bullets.begin() + i--);
            if (enemies[j].hp <= 0)
            {
                // spawn power-up z enumem
                if (rand() % 4 == 0)
                {
                    PowerUpType type = static_cast<PowerUpType>(rand() % 3 + 1);
                    powerUps.emplace_back(
                        enemies[j].shape.getPosition().x,
                        enemies[j].shape.getPosition().y,
                        type
                    );
                }
                enemies.erase(enemies.begin() + j);
            }
            break;
        }
    }
}


    // EnemyBullet -> Player
    for (size_t i = 0; i < enemyBullets.size(); i++)
    {
        if (intersects(enemyBullets[i].shape.getGlobalBounds(),
                       gracz->getGlobalBounds()))
        {
            if (!shield) this->okno->close();
            enemyBullets.erase(enemyBullets.begin() + i--);
        }
    }

    // PowerUp -> Player
    for (size_t i = 0; i < powerUps.size(); i++)
    {
        if (intersects(powerUps[i].shape.getGlobalBounds(),
                       gracz->getGlobalBounds()))
        {
            if (powerUps[i].type == 1) { shootCooldown = 0.15f; fastShotTimer = POWERUP_DURATION; }
            if (powerUps[i].type == 2) { doubleShot = true; doubleShotTimer = POWERUP_DURATION; }
            if (powerUps[i].type == 3) { shield = true; shieldTimer = POWERUP_DURATION; }
            powerUps.erase(powerUps.begin() + i);
            break;
        }
    }

    // ------------------------------
    // SPAWNY FALI
    // ------------------------------
    if (enemies.empty())
    {
        waveManager->nextWave();
    }
}

// ============================
// RENDER
// ============================

void game::render()
{
    this->okno->clear(sf::Color::Black);

    // render gracza
    this->gracz->render_Player(*okno);

    // render obiektów Galagi
    for (auto& b : bullets) this->okno->draw(b.shape);
    for (auto& eb : enemyBullets) this->okno->draw(eb.shape);
    for (auto& e : enemies) this->okno->draw(e.shape);
    for (auto& p : powerUps) this->okno->draw(p.shape);

    // BOSS HP
    waveManager->drawBossHP(*okno);

    // ============================
    // Power-up bars nad graczem
    // ============================
    sf::Vector2f barPos(gracz->getX() - 25.f, gracz->getY() - 20.f); // start lewo od środka gracza
    float yOffset = 0.f;

    if (fastShotTimer > 0.f)
    {
        float width = 50.f * (fastShotTimer / POWERUP_DURATION);
        powerUpBar.setSize({width, 5.f});
        powerUpBar.setFillColor(sf::Color::Cyan);
        powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
        okno->draw(powerUpBar);

        powerUpText.setString(std::to_string((int)fastShotTimer) + "s");
        powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
        okno->draw(powerUpText);

        yOffset += 10.f;
    }

    if (doubleShotTimer > 0.f)
    {
        float width = 50.f * (doubleShotTimer / POWERUP_DURATION);
        powerUpBar.setSize({width, 5.f});
        powerUpBar.setFillColor(sf::Color::Magenta);
        powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
        okno->draw(powerUpBar);

        powerUpText.setString(std::to_string((int)doubleShotTimer) + "s");
        powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
        okno->draw(powerUpText);

        yOffset += 10.f;
    }

    if (shieldTimer > 0.f)
    {
        float width = 50.f * (shieldTimer / POWERUP_DURATION);
        powerUpBar.setSize({width, 5.f});
        powerUpBar.setFillColor(sf::Color::Green);
        powerUpBar.setPosition(barPos.x, barPos.y - yOffset);
        okno->draw(powerUpBar);

        powerUpText.setString(std::to_string((int)shieldTimer) + "s");
        powerUpText.setPosition(barPos.x + width + 5.f, barPos.y - yOffset - 2.f);
        okno->draw(powerUpText);

        yOffset += 10.f;
    }
// ============================
// HUD LEWY GÓRNY RÓG
// ============================
sf::Vector2f hudPos(10.f, 10.f); // lewy górny róg
float hudYOffset = 0.f; // odstęp w pionie
float radius = 8.f;     // rozmiar kółka

auto drawHUDPowerUp = [&](PowerUpType type, const std::string& name, float timer)
{
    if (timer > 0.f)
    {
        sf::CircleShape circle(radius);
        circle.setFillColor(PowerUp(0,0,type).getColor());
        circle.setPosition(hudPos.x, hudPos.y + hudYOffset);
        okno->draw(circle);

        powerUpText.setString(name);
        powerUpText.setPosition(hudPos.x + 20.f, hudPos.y + hudYOffset - 2.f);
        okno->draw(powerUpText);

        hudYOffset += 20.f;
    }
};

// Fast shot
drawHUDPowerUp(FAST_SHOT, "Fast Shot", fastShotTimer);

// Double shot
drawHUDPowerUp(DOUBLE_SHOT, "Double Shot", doubleShotTimer);

// Shield
drawHUDPowerUp(SHIELD, "Shield", shieldTimer);


    this->okno->display();
}
