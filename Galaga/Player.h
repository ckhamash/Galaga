#pragma once
#include "SFML\Graphics.hpp"
#include "Bullet.h"

class Player {
	sf::Sprite sprite;
	//Bullet bullet;

public:
	Player();
	Player(sf::Texture& shipTexture, sf::Vector2f position);
	~Player();

	const sf::Sprite& getSprite() const;
};

