#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "WaveManager.h"
#include "PowerUp.h"
#include "PowerUpManager.h"
#include "Collision.h"

// ZAKTUALIZOWANE: Dodano stan LOGIN
enum class GameState { MENU, LOGIN, PLAYING, GAME_OVER, PROFILE };

class game
{
private:
    sf::RenderWindow* okno;
    sf::Event event;

    // Gracz i logika
    Player* gracz;
    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enemyBullets;
    std::vector<Enemy> enemies;

    // Managery
    WaveManager* waveManager;
    PowerUpManager* powerUpManager;

    // PowerUpy
    std::vector<PowerUp> powerUps;
    bool doubleShot = false;
    bool shield = false;
    float shootCooldown = 0.25f;
    float shootTimer = 0.f;

    // Timery PowerUpów
    float doubleShotTimer = 0.f;
    float fastShotTimer = 0.f;
    float shieldTimer = 0.f;
    const float POWERUP_DURATION = 5.f;

    // UI / GUI
    sf::Font font;
    sf::Text powerUpText;
    sf::RectangleShape powerUpBar;

    // --- LOGO ---
    sf::Texture logoTexture;
    sf::Sprite logoSprite;

    // --- PRZYCISKI MENU ---
    sf::RectangleShape btnStart;
    sf::Text textStart;

    sf::RectangleShape btnProfile;
    sf::Text textProfile;

    // System Stanów i Zapisu
    GameState state;
    std::string inputNick;

    // NOWA ZMIENNA: Zapamiętuje, gdzie gracz chciał wejść po wpisaniu nicku
    GameState nextStateAfterLogin;

    // Tło
    int bgID; // 0=black, 1=fiolet, 2=granat, 3=braz
    sf::Color backgroundColor;

    // Funkcje inicjalizujące
    void initZmienne();
    void initOkno();
    void initGracz();
    void initManagers();
    void initHUD();
    void initButtons();

public:
    game();
    virtual ~game();

    const bool OknoOtwarte() const;

    // Funkcje Główne
    void updateEvent();
    void update();
    void render();

    // Funkcje Pomocnicze Renderowania
    void renderMenu();
    void renderLogin(); // NOWA FUNKCJA: Ekran wpisywania nicku
    void renderProfile();
    void renderHUD();
    void renderEndScreen();

    // Funkcje Zapisu/Odczytu
    void saveGame();
    void loadGame();

    // Setter tła
    void setBackground(int id);
};