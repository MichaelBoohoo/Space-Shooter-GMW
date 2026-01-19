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

    // Inicjalizacja skórek (4 skiny: 0, 1, 2, 3)
    // Skin 0 (domyślny) jest odblokowany (1), reszta zablokowana (0)
    this->ownedSkins.clear();
    this->ownedSkins.push_back(1); // Skin 0
    this->ownedSkins.push_back(0); // Skin 1
    this->ownedSkins.push_back(0); // Skin 2
    this->ownedSkins.push_back(0); // Skin 3
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

void Player::resetAllStats()
{
    this->initZmienne_Player();
    this->zmienSkorke(0); // Reset koloru
}

// --- SYSTEM SKÓREK ---
bool Player::czyPosiadaSkin(int id) const
{
    if (id < 0 || id >= this->ownedSkins.size()) return false;
    return (this->ownedSkins[id] == 1);
}

void Player::odblokujSkin(int id)
{
    if (id >= 0 && id < this->ownedSkins.size()) {
        this->ownedSkins[id] = 1;
    }
}
// ---------------------

void Player::zmienSkorke(int id) {
    // Zabezpieczenie: można zmienić tylko na posiadany skin
    if (!czyPosiadaSkin(id)) return;

    this->colorID = id;
    this->currentSkinID = id;
    std::string fileName = "skin" + std::to_string(id) + ".png";

    // Jeśli id = 0, szuka skin0.png, jeśli 1 to skin1.png itd.
    if (this->texture.loadFromFile(fileName)) {
        this->kwadrat.setTexture(&this->texture);
        this->kwadrat.setFillColor(sf::Color::White); // Resetujemy kolor na biały
    }
    else {
        // Jeśli nie znajdzie pliku, ustawia kolor zastępczy
        this->kwadrat.setTexture(nullptr);
        if (id == 0) this->kwadrat.setFillColor(sf::Color::Green);
        else if (id == 1) this->kwadrat.setFillColor(sf::Color::Blue);
        else if (id == 2) this->kwadrat.setFillColor(sf::Color::Yellow);
        else this->kwadrat.setFillColor(sf::Color::Magenta);
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

const sf::FloatRect Player::getGlobalBounds() const { return this->kwadrat.getGlobalBounds(); }
const sf::Vector2f Player::getPosition() const { return this->kwadrat.getPosition(); }
const float Player::getX() const { return this->kwadrat.getPosition().x; }
const float Player::getY() const { return this->kwadrat.getPosition().y; }

const int Player::getHP() const { return this->hp; }
const int Player::getHPMax() const { return this->hpMax; }
const int Player::getPunkty() const { return this->punkty; }
const std::string Player::getNick() const { return this->nick; }

const int Player::getSpeedLevel() const { return this->speedLevel; }
const int Player::getHpLevel() const { return this->hpLevel; }
const int Player::getSkinID() const { return this->colorID; }

void Player::setNick(std::string n) { this->nick = n; }
void Player::setPunkty(int p) { this->punkty = p; }
void Player::setHpMax(int h) { this->hpMax = h; this->hp = h; }
void Player::setSpeed(float s) { this->movementSpeed = s; }

void Player::dodajPunkty(int p) { this->punkty += p; }
void Player::wydajPunkty(int p) { this->punkty -= p; if (this->punkty < 0) this->punkty = 0; }

void Player::otrzymajObrazenia(int damage)
{
    this->hp -= damage;
    if (this->hp < 0) this->hp = 0;
}

void Player::reset()
{
    this->hp = this->hpMax;
}

void Player::ulepszHP()
{
    this->hpMax += 5;
    this->hp = this->hpMax; // Leczy przy ulepszeniu
    this->hpLevel++;
}

void Player::ulepszSpeed()
{
    this->movementSpeed += 1.f;
    this->speedLevel++;
}