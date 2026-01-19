#include "Player.h"




void Player::initZmienne_Player()
{
    this->movementSpeed = 5.f; //predkosc na 5 pikseli na klatke
    this->hpMax = 10;
    this->hp = this->hpMax;
    this->punkty = 0;
    this->nick = "Gracz";

    // Domyœlne wartoœci
    this->colorID = 0; // 0 = Zielony
    this->speedLevel = 1;
    this->hpLevel = 1;
    
}

void Player::initKsztalt()
{
    //kwadrat o wymiarach 50x50
    this->kwadrat.setSize(sf::Vector2f(50.f, 50.f));

    
    this->zmienSkorke(0); // zobaczyc czy odwolac sie trza do kwadratu
}

// Konstruktor i Destruktor 

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
    
}

// Funkcja gdy wpiszemy nowy nick, którego nie ma w plikach
void Player::resetAllStats()
{
    this->initZmienne_Player();
    this->zmienSkorke(0); // Reset koloru
}

void Player::zmienSkorke(int id) {
    this->currentSkinID = id;
    std::string fileName = "../Assets/skin" + std::to_string(id) + ".png";
    // Jeœli id = 0, szuka skin0.png, jeœli 1 to skin1.png itd.

    if (this->texture.loadFromFile(fileName)) {
        this->kwadrat.setTexture(&this->texture);
        this->kwadrat.setFillColor(sf::Color::White); // Resetujemy kolor na bia³y, by nie barwi³ tekstury
    }
    else {
        // Jeœli nie znajdzie pliku, ustawia kolor zastêpczy
        this->kwadrat.setTexture(nullptr);
        if (id == 0) this->kwadrat.setFillColor(sf::Color::Green);
        else this->kwadrat.setFillColor(sf::Color::Blue);
    }
}



void Player::update_Player()
{
    this->currentSpeed = sf::Vector2f(0.f, 0.f);
    //ruch w lewo 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->kwadrat.move(-this->movementSpeed, 0.f);
        this->currentSpeed.x = -this->movementSpeed;
    }
    //ruch w prawo 
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->kwadrat.move(this->movementSpeed, 0.f);
        this->currentSpeed.x = this->movementSpeed;
    }

    //ruch w gore
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->kwadrat.move(0.f, -this->movementSpeed);
        this->currentSpeed.y = -this->movementSpeed;
    }
    //ruch w dol
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->kwadrat.move(0.f, this->movementSpeed);
        this->currentSpeed.y = this->movementSpeed;
    }
}

void Player::render_Player(sf::RenderTarget& okno)
{
    //rysowanie kwadratu na konie
    okno.draw(this->kwadrat);
}

