#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class Player
{
private:
	sf::RectangleShape kwadrat; //gracz
	sf::Vector2f currentSpeed;
	sf::Texture texture;
	float movementSpeed; // szybkosc poruszania sie
	int currentSkinID;

	int hp;
	int hpMax;
	int punkty;

	std::string nick;

	// Zmienne do zapisu i sklepu

	int colorID;    // 0 = zielony, 1 = niebieski, 2 = zolty etc.
	int speedLevel; // poziom szybkosci
	int hpLevel;    // poziom ulepszenia HP



	void initZmienne_Player(); //funkcja do zmiennych w tej klasie
	void initKsztalt();

public:

	//Konstruktor i destruktor
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

	// Funkcje modyfikuj¹ce (np. przy wczytywaniu gry)
	void setPunkty(int v) { this->punkty = v; }
	void setHpMax(int v) { this->hpMax = v; }
	void setSpeed(float v) { this->movementSpeed = v; }

	// Zmiana wygl¹du
	void zmienSkorke(int id); // Zmieniamy nazwê z zmienKolor na zmienSkorke
	
	// Logika gry
	void dodajPunkty(int ilosc) { this->punkty += ilosc; }
	void wydajPunkty(int ilosc) { this->punkty -= ilosc; }
	void otrzymajObrazenia(int ilosc) { this->hp -= ilosc; if (this->hp < 0) this->hp = 0; }
	void ulecz(int ilosc) { this->hp += ilosc; if (this->hp > this->hpMax) this->hp = this->hpMax; }

	// Zakup ulepszeñ
	void ulepszSpeed() { this->speedLevel++; this->movementSpeed += 1.f; }
	void ulepszHP() { this->hpLevel++; this->hpMax += 5; this->hp = this->hpMax; }

	
	// Resetuje HP do max (dla nowej rundy)
	void reset() { this->hp = this->hpMax; }
	// NOWA FUNKCJA: Resetuje wszystko do zera (dla nowego profilu)
	void resetAllStats();

	//funkcje
	void update_Player();
	void render_Player(sf::RenderTarget& okno);
    sf::RectangleShape& getShape() { return kwadrat; }

    // Gettery pozycji i kolizji

    float getX() const { return kwadrat.getPosition().x; }
    float getY() const { return kwadrat.getPosition().y; }

	sf::Vector2f getSpeed() { return currentSpeed; }

    sf::FloatRect getGlobalBounds() const { return kwadrat.getGlobalBounds(); }

};
