#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "WaveManager.h"
#include "PowerUpManager.h"
#include "Collision.h"

class game
{
private:
    // ============================
    // Zmienne
    // ============================

    // okno
    sf::RenderWindow* okno;
    sf::Event event;

    // gracz
    Player* gracz;

    // ------------------------------
    // Kolekcje obiektów Galagi
    // ------------------------------
    std::vector<Bullet> bullets;             // pociski gracza
    std::vector<EnemyBullet> enemyBullets;   // pociski wroga
    std::vector<Enemy> enemies;              // wrogowie
    std::vector<PowerUp> powerUps;           // bonusy

    // ------------------------------
    // Timery i statusy power-up
    // ------------------------------
    bool doubleShot = false;  // podwójny strzał
    bool shield = false;      // tarcza
    float shootCooldown = 0.4f; // czas między strzałami
    float shootTimer = 0.f;     // licznik strzału

    float doubleShotTimer = 0.f;   
    float fastShotTimer = 0.f;     
    float shieldTimer = 0.f;       
    const float POWERUP_DURATION = 5.f; // czas trwania power-upu w sekundach

    // ============================
    // Menedżery
    // ============================
    WaveManager* waveManager = nullptr;       // menedżer fal
    PowerUpManager* powerUpManager = nullptr; // menedżer power-upów

    // ============================
    // Tekstury / HUD
    // ============================
    sf::Font font;        // font do wyświetlania timerów
    sf::Text powerUpText; // tekst do wyświetlania timerów
    sf::RectangleShape powerUpBar; // pasek power-upa (opcjonalnie)

    // ============================
    // Prywatne funkcje inicjalizacji
    // ============================
    void initZmienne();   // inicjalizacja zmiennych klasy
    void initOkno();      // funkcja do tworzenia okna
    void initGracz();     // funkcja do tworzenia gracza
    void initManagers();  // inicjalizacja menedżerów fal i power-upów
    void initHUD();       // inicjalizacja fontu i tekstu

public:
    // ============================
    // Konstruktor i destruktor
    // ============================
    game();
    virtual ~game();

    // ============================
    // Dostęp
    // ============================
    const bool OknoOtwarte() const;

    // ============================
    // Funkcje gry
    // ============================
    void updateEvent(); // obsługa zdarzeń
    void update();      // update logiki gry
    void render();      // renderowanie obiektów
};//srednik na koncu wymagany by zaznaczyc gdzie sie konczy opis klasy