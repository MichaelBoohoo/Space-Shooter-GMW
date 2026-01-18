#include "Player.h"

void Player::initZmienne_Player()
{
    this->movementSpeed = 5.f;
    this->hpMax = 10;
    this->hp = this->hpMax;
    this->punkty = 0;
    this->nick = "Gracz";

    // Domyślne wartości
    this->colorID = 0; // 0 = Zielony
    this->speedLevel = 1;
    this->hpLevel = 1;
}

void Player::initKsztalt()
{
    this->kwadrat.setSize(sf::Vector2f(50.f, 50.f));
    this->zmienSkorke(0); // Ustaw domyślny kolor
}

Player::Player(float x, float y)
{
    this->initZmienne_Player(); // Najpierw zmienne
    this->initKsztalt();        // Potem kształt
    this->kwadrat.setPosition(x, y);
}

Player::~Player()
{
}

// --- NOWA FUNKCJA ---
// Używamy jej, gdy wpiszemy nowy nick, którego nie ma w plikach
void Player::resetAllStats()
{
    this->initZmienne_Player();
    this->zmienSkorke(0); // Reset koloru
}

void Player::zmienSkorke(int id) {
    this->currentSkinID = id;
    std::string fileName = "skin" + std::to_string(id) + ".png";
    // Jeśli id = 0, szuka skin0.png, jeśli 1 to skin1.png itd.

    if (this->texture.loadFromFile(fileName)) {
        this->kwadrat.setTexture(&this->texture);
        this->kwadrat.setFillColor(sf::Color::White); // Resetujemy kolor na biały, by nie barwił tekstury
    }
    else {
        // Jeśli nie znajdzie pliku, ustawia kolor zastępczy
        this->kwadrat.setTexture(nullptr);
        if (id == 0) this->kwadrat.setFillColor(sf::Color::Green);
        else this->kwadrat.setFillColor(sf::Color::Blue);
    }
}

void Player::update_Player()
{
    // Ruch
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->kwadrat.move(-this->movementSpeed, 0.f);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->kwadrat.move(this->movementSpeed, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->kwadrat.move(0.f, -this->movementSpeed);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->kwadrat.move(0.f, this->movementSpeed);

    // Ograniczenie ekranu
    if (this->kwadrat.getPosition().x < 0) this->kwadrat.setPosition(0, this->kwadrat.getPosition().y);
    if (this->kwadrat.getPosition().x > 750) this->kwadrat.setPosition(750, this->kwadrat.getPosition().y);
}

void Player::render_Player(sf::RenderTarget& okno)
{
    okno.draw(this->kwadrat);
}
sf::RectangleShape& Player::getShape()
{
    return this->kwadrat;
}