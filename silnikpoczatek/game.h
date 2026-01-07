#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.h"

// klasa ktora dziala jako silnik 



class game
{
private:
	//zmienne
	//okno
	sf::RenderWindow* okno;
	sf::Event event;

	Player* gracz;

//prywatne funkcje
	void initZmienne();
	void initOkno();//funkcja do tworzenia okna
	void initGracz();// funkcja do tworzenia gracza

public:
	//konstruktory i destruktory
	game();
	virtual ~game();

	//dostep
	const bool OknoOtwarte() const;

	//Funkcje
	void updateEvent();
	void update();
	void render();
};//srednik na koncu wymagany by zaznaczyc gdzie sie konczy opis klasy