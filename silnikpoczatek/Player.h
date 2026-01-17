#pragma once
#include <SFML/Graphics.hpp>
class Player
{
private:
	sf::RectangleShape kwadrat; //gracz
	float movementSpeed; // szybkosc poruszania sie

	void initZmienne_Player(); //funkcja do zmiennych w tej klasie
	void initKsztalt();

public:

	//Konstruktor i destruktor
	Player(float x = 0.f, float y = 0.f);

	virtual ~Player();

	//funkcje
	void update_Player();
	void render_Player(sf::RenderTarget& okno);
};

