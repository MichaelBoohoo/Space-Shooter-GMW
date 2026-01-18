#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player
{
private:
    sf::RectangleShape kwadrat;
    sf::Texture texture;
    int currentSkinID;
    float movementSpeed;


    int hp;
    int hpMax;
    int punkty;

    std::string nick;

    // --- ZMIENNE DO ZAPISU I SKLEPU ---
    int colorID;    // 0 = zielony, 1 = niebieski, 2 = zolty etc.
    int speedLevel; // poziom szybkosci
    int hpLevel;    // poziom ulepszenia HP

    void initZmienne_Player();
    void initKsztalt();

public:
    Player(float x = 0.f, float y = 0.f);
    virtual ~Player();

    // Settery i Gettery
    void setNick(std::string n) { this->nick = n; }
    std::string getNick() const { return this->nick; }

    const int& getHP() const { return this->hp; }
    const int& getHPMax() const { return this->hpMax; }
    const int& getPunkty() const { return this->punkty; }

    // Funkcje dla sklepu i zapisu
    int getSkinID() const { return this->colorID; }
    int getSpeedLevel() const { return this->speedLevel; }
    int getHpLevel() const { return this->hpLevel; }

    // Funkcje modyfikujące (np. przy wczytywaniu gry)
    void setPunkty(int v) { this->punkty = v; }
    void setHpMax(int v) { this->hpMax = v; }
    void setSpeed(float v) { this->movementSpeed = v; }

    // Zmiana wyglądu
    void zmienSkorke(int id); // Zmieniamy nazwę z zmienKolor na zmienSkorke
    sf::RectangleShape& getShape();

    // Logika gry
    void dodajPunkty(int ilosc) { this->punkty += ilosc; }
    void wydajPunkty(int ilosc) { this->punkty -= ilosc; }
    void otrzymajObrazenia(int ilosc) { this->hp -= ilosc; if (this->hp < 0) this->hp = 0; }
    void ulecz(int ilosc) { this->hp += ilosc; if (this->hp > this->hpMax) this->hp = this->hpMax; }

    // Zakup ulepszeń
    void ulepszSpeed() { this->speedLevel++; this->movementSpeed += 1.f; }
    void ulepszHP() { this->hpLevel++; this->hpMax += 5; this->hp = this->hpMax; }

    // --- RESETOWANIE ---
    // Resetuje HP do max (dla nowej rundy)
    void reset() { this->hp = this->hpMax; }
    // NOWA FUNKCJA: Resetuje wszystko do zera (dla nowego profilu)
    void resetAllStats();

    void update_Player();
    void render_Player(sf::RenderTarget& okno);

    // Gettery do kształtu (dla kolizji)

    float getX() const { return kwadrat.getPosition().x; }
    float getY() const { return kwadrat.getPosition().y; }
    sf::FloatRect getGlobalBounds() const { return kwadrat.getGlobalBounds(); }
};