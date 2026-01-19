#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
class Audio
{
private:
	std::map<std::string, sf::SoundBuffer> buffer;

	std::map<std::string, sf::Sound> sound;

public:
	Audio();
	virtual ~Audio();

	void loadSound(std::string name, std::string route);

	void playSound(std::string name);
};

