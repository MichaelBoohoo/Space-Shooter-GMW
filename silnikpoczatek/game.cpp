#include "game.h"



//funkcje prywatne
void game::initZmienne()
{
	this->okno = nullptr;
}

void game::initOkno()
{
	this->okno = new sf::RenderWindow(sf::VideoMode(800, 600), "Space Shooter");
	this->okno -> setFramerateLimit(60);
}

void game::initGracz()
{
	this->gracz = new Player();
}

//konstruktory i destruktory
game::game()
{
	this->initZmienne();
	this->initOkno();
	this->initGracz();

}

game::~game()
{
	delete this->okno;//by nie bylo wycieku pamieci
	delete this->gracz;//usuwanie gracza z pamieci przy wyjsciu
}

//Dostep
const bool game::OknoOtwarte() const
{
	return this->okno->isOpen();
}
void game::updateEvent()
{
	// obsluga zdarzen
	while (this->okno->pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed)
			this->okno->close();

		else if (this->event.key.code == sf::Keyboard::Escape)
			this->okno->close();
	}
}
//funkcje
void game::update()
{
	this->updateEvent();

	this->gracz->update_Player();
}

void game::render()//renderuje obiekty w grze 
{
	this->okno->clear(sf::Color::Black);

	this->gracz->render_Player(*this->okno);

	this->okno->display();
}

