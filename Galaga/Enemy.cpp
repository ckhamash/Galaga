#include "Enemy.h"
#include <iostream>
Enemy::Enemy() {}

Enemy::Enemy(const sf::Texture& texture, std::vector<Path> p, sf::Vector2f position, int score, int healthPoints)
	: sprite(texture)
	, paths(p)
	, formationPosition(position)
	, value(score)
	, health(healthPoints)
	, laserReloadtime(0.5f)
	, numShots(0){
}

Enemy::~Enemy() {}

sf::Sprite& Enemy::getSprite() { return sprite; }

sf::Vector2f Enemy::getPosition() { return formationPosition; }

int Enemy::getValue() { 
	if (!pathsFinished() && sprite.getPosition() != formationPosition)
		return value * 2;
	else
		return value;
}

int Enemy::getHealth() { return health; }

int& Enemy::shotsLeft() { return numShots; }

void Enemy::setPaths(std::vector<Path> &p) { paths = p; }

bool Enemy::isDead() { return health < 1; }

void Enemy::move(float secondsSinceLastFrame) { 
	paths[currentPath].moveToNextPosition(sprite, secondsSinceLastFrame);
	if (currentPath < paths.size() - 1 && paths[currentPath].isDone()) {
		currentPath++;
		paths[currentPath].start();
	}
	else if (pathsFinished())
		sprite.setPosition(formationPosition);
}

void Enemy::start() { 
	paths[0].start();
	laserReloadtimer.restart();
	currentPath = 0;
}

bool Enemy::pathsFinished() { return currentPath == paths.size() - 1 && paths[currentPath].isDone(); }

bool Enemy::canShoot() { return (numShots > 0 && paths[currentPath].canShoot() && laserReloadtime < laserReloadtimer.getElapsedTime().asSeconds()); }

void Enemy::fire() {
	laserReloadtimer.restart();
	numShots--;
}

void Enemy::hit() {	health--; }