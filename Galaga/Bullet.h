#pragma once
#include "SFML\Graphics.hpp"

class Bullet {
	sf::Sprite sprite;
	sf::Vector2f velocity;

public:
	Bullet();
	Bullet(sf::Texture& texture, sf::Vector2f velocity);
	~Bullet();

	sf::Sprite& getSprite();

	void move(float secondsSinceLastFrame);
};

