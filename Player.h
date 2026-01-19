#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Player
{
private:
    sf::RectangleShape kwadrat;
    sf::Texture texture;

    float movementSpeed;

    // Statystyki
    int hp;
    int hpMax;
    int punkty;

    // Poziomy ulepszeń
    int speedLevel;
    int hpLevel;

    // Wygląd
    int colorID;       // Aktualny kolor/skin ID
    int currentSkinID; // Pomocnicza do logiki

    // System skórek - wektor przechowywujący stan (0 - nieposiadany, 1 - posiadany)
    std::vector<int> ownedSkins;

    // Dane gracza
    std::string nick;

    void initZmienne_Player();
    void initKsztalt();

public:
    Player(float x, float y);
    virtual ~Player();

    // Resetowanie
    void reset();           // Resetuje HP (po wczytaniu/śmierci)
    void resetAllStats();   // Resetuje wszystko do zera (nowy gracz)

    // Accessors (Gettery)
    sf::RectangleShape& getShape();
    const sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f getPosition() const;
    const float getX() const;
    const float getY() const;

    const int getHP() const;
    const int getHPMax() const;
    const int getPunkty() const;
    const std::string getNick() const;

    // Do zapisu gry
    const int getSpeedLevel() const;
    const int getHpLevel() const;
    const int getSkinID() const;

    // --- NOWE METODY DO SKÓREK ---
    bool czyPosiadaSkin(int id) const;
    void odblokujSkin(int id);
    // -----------------------------

    // Modifiers (Settery / Akcje)
    void setNick(std::string n);
    void setPunkty(int p);
    void setHpMax(int h);
    void setSpeed(float s);

    void dodajPunkty(int p);
    void wydajPunkty(int p);
    void otrzymajObrazenia(int damage);

    // Ulepszenia
    void ulepszHP();
    void ulepszSpeed();

    // Zmiana wyglądu
    void zmienSkorke(int id);

    // Update & Render
    void update_Player();
    void render_Player(sf::RenderTarget& okno);
};