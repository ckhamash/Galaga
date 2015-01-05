#include "Bullet.h"


Bullet::Bullet() {}

Bullet::Bullet(sf::Texture& texture, sf::Vector2f vel) 
	: sprite(texture)
	, velocity(vel) {
}

Bullet::~Bullet() {}

sf::Sprite& Bullet::getSprite() { return sprite; }

void Bullet::move(float secondsSinceLastFrame) { sprite.move(velocity * secondsSinceLastFrame); }