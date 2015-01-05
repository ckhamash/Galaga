#pragma once
#include "SFML\Graphics.hpp"
#include "Path.h"
#include "Bullet.h"

class Enemy {
private:
	std::vector<Path> paths;

	sf::Sprite sprite;
	int value, numShots, health;
	int currentPath = 0;
	sf::Vector2f formationPosition;

	sf::Clock laserReloadtimer;
	float laserReloadtime;
public:
	Enemy();
	Enemy(const sf::Texture& texture, std::vector<Path> p, sf::Vector2f position, int score, int healthPoints);
	~Enemy();

	sf::Sprite& getSprite();
	sf::Vector2f getPosition();
	int getValue();
	int getHealth();
	int& shotsLeft();
	void setPaths(std::vector<Path> &p);

	bool isDead();

	void move(float secondsSinceLastFrame);
	void start();

	bool pathsFinished();
	bool canShoot();
	void fire();
	void hit();
};

