#include "Audio.h"
#include <iostream>

Audio::Audio()
{
	// TYLKO TUTAJ DODAWAC DZWIEKI
	// miejsce do dodawania buforow dzwiekow powolujac sie na loadSound()
	this->loadSound("STRZAL", "../Assets/strzal.wav");
	this->loadSound("ENEMY-1", "../Assets/zombie-1.wav");
	this->loadSound("ENEMY-2", "../Assets/zombie-2.wav");
	this->loadSound("ENEMY-3", "../Assets/zombie-3.wav");
	this->loadSound("ENEMY-4", "../Assets/zombie-4.wav");
	this->loadSound("CLICK", "../Assets/click.wav");
	this->loadSound("ZEUS", "../Assets/laugh.mp3");
}

Audio::~Audio()
{
}

void Audio::loadSound(std::string name, std::string route)
{
	
	sf::SoundBuffer tempBuffer;
	
	sf::Sound tempSound;
	if (tempBuffer.loadFromFile(route)) {
		
		this->buffer[name] = tempBuffer;
		
		tempSound.setBuffer(this->buffer[name]);
		tempSound.setVolume(50.f); // glosnosc domyslna

		
		this->sound[name] = tempSound;
	}
}

void Audio::playSound(std::string name)
{
	// sprawdzenie, czy taki dzwiek wystepuje w mapie
	if (this->sound.find(name) != sound.end()) { 
		this->sound[name].stop();
		this->sound[name].play();
	}
	else {
		std::cout << "BLAD: nie znaleziono pliku (.wav) o nazwie '" << name << "'" << std::endl;//na wszelki wypadek by wiedziec gdzie szukac
	}
}
