#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    sf::RectangleShape shape;
    int type;
    int hp;
    float shootTimer = 0.f;

    // Statyczne tekstury - wspólne dla wszystkich wrogów
    static sf::Texture t1; // dla zly1.png
    static sf::Texture t2; // dla zly2.png
    static sf::Texture t3; // dla zly3.png (boss)
    static bool texturesLoaded; // flaga czy już załadowano
    static void loadTextures(); // funkcja ładująca

    Enemy(float x, float y, int t);
    void update(float dt);
};