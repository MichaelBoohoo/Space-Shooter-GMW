#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>
#include <sstream>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "WaveManager.h"
#include "PowerUpManager.h"
#include "Audio.h"

enum class GameState { MENU, LOGIN, PLAYING, GAME_OVER, PROFILE };

class game
{
private:
    
    // Zmienne
    
    int sound_rand = 0;//zmienna do przechowywania losowej wartosci
    // okno
    sf::RenderWindow* okno;
    sf::Event event;

    // gracz
    Player* gracz;

    // audio
    Audio audio;
    
    // Kolekcje obiektów 
    
    std::vector<Bullet> bullets;             // pociski gracza
    std::vector<EnemyBullet> enemyBullets;   // pociski wroga
    std::vector<Enemy> enemies;              // wrogowie
    std::vector<PowerUp> powerUps;           // bonusy

    // Okno dialogowe
    bool isDialogOn = false;
    sf::RectangleShape dialogBox;
    sf::Text dialogText;
    sf::Text dialogName;

    std::vector<std::string> iloscDialog;
    int ktoryDialog = 0;
    int losowyDialog = 0;
    
    // Timery i statusy power-up
    
    bool doubleShot = false;  // podwójny strzał
    bool shield = false;      // tarcza
    float shootCooldown = 0.4f; // czas między strzałami
    float shootTimer = 0.f;     // licznik strzału

    float doubleShotTimer = 0.f;   
    float fastShotTimer = 0.f;     
    float shieldTimer = 0.f;       
    const float POWERUP_DURATION = 5.f; // czas trwania power-upu w sekundach

    

    // Menedżery
    
    WaveManager* waveManager = nullptr;       // menedżer fal
    PowerUpManager* powerUpManager = nullptr; // menedżer power-upów

    
    // Tekstury / HUD
    
    sf::Font font;        // font do wyświetlania timerów
    sf::Text powerUpText; // tekst do wyświetlania timerów
    sf::RectangleShape powerUpBar; // pasek power-upa (opcjonalnie)

    // Logo
    sf::Texture logoTexture;
    sf::Sprite logoSprite;

    //  Przyciski menu
    sf::RectangleShape btnStart;
    sf::Text textStart;

    sf::RectangleShape btnProfile;
    sf::Text textProfile;

    // System Stanów i Zapisu
    GameState state;
    std::string inputNick;

    // Gdzie gracz chciał wejść po wpisaniu nicku
    GameState nextStateAfterLogin;

    // Tło
    int bgID; // 0=black, 1=fiolet, 2=granat, 3=braz
    sf::Color backgroundColor;

    
    // Prywatne funkcje inicjalizacji
    
    void initZmienne();    
    void initOkno();      // funkcja do tworzenia okna
    void initGracz();     // funkcja do tworzenia gracza
    void initManagers();  // inicjalizacja menedżerów fal i power-upów
    void initHUD();       // inicjalizacja fontu i tekstu
    void initButtons();
    void initDialogs();

public:
    
    // Konstruktor i destruktor
    
    game();
    virtual ~game();

    
    // Dostęp
    
    const bool OknoOtwarte() const;

    
    // Funkcje gry
    
    void updateEvent(); // obsługa zdarzeń
    void update();      // update logiki gry
    void render();      // renderowanie obiektów

    // Funkcje Pomocnicze Renderowania
    void renderMenu();
    void renderLogin(); // ekran wpisywania nicku
    void renderProfile();
    void renderHUD();
    void renderEndScreen();

    // Funkcje Zapisu/Odczytu
    void saveGame();
    void loadGame();

    // Setter tła
    void setBackground(int id);

    //Funkcja do wywolywania dialogu
    void startDialog(std::vector<std::string> lines, std::string name = "System");

};//srednik na koncu wymagany by zaznaczyc gdzie sie konczy opis klasy