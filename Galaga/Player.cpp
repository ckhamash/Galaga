#include "Player.h"


Player::Player() {}

Player::Player(sf::Texture& shipTexture, sf::Vector2f position) {
	sprite = sf::Sprite(shipTexture);
	sprite.setPosition(position);
}

Player::~Player() {}

const sf::Sprite& Player::getSprite() const { return sprite; }