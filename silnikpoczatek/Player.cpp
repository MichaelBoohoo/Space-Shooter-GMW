#include "Player.h"




void Player::initZmienne_Player()
{
    this->movementSpeed = 5.f; //predkosc na 5 pikseli na klatke
}

void Player::initKsztalt()
{
    //kwadrat o wymiarach 50x50
    this->kwadrat.setSize(sf::Vector2f(50.f, 50.f));

    
    this->kwadrat.setFillColor(sf::Color::Green);
}

// --- Konstruktor i Destruktor ---

Player::Player(float x, float y)
{
    //wywolujemy funkcje pomocnicze
    this->initKsztalt();
    this->initZmienne_Player();

    //ustawiamy pozycje startowa
    this->kwadrat.setPosition(x, y);
}

Player::~Player()
{
    // Tutaj czyœcilibyœmy pamiêæ, ale na razie nie jest to potrzebne
}



void Player::update_Player()
{

    //ruch w lewo 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->kwadrat.move(-this->movementSpeed, 0.f);
    }
    //ruch w prawo 
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->kwadrat.move(this->movementSpeed, 0.f);
    }

    //ruch w gore
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->kwadrat.move(0.f, -this->movementSpeed);
    }
    //ruch w dol
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->kwadrat.move(0.f, this->movementSpeed);
    }
}

void Player::render_Player(sf::RenderTarget& okno)
{
    //rysowanie kwadratu na konie
    okno.draw(this->kwadrat);
}