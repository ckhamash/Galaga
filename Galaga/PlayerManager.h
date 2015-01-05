#pragma once
#include "SFML\Graphics.hpp"
#include "Bullet.h"
#include <vector>
#include <iostream>

class Game;

class PlayerManager {
	Game *parentGame;

	sf::Texture shipTexture, bulletTexture;

	std::vector<sf::Sprite> playerShips;
	std::vector<Bullet> playerBullets;
	float playerMoveSpeed, bulletMoveSpeed;

	sf::Clock laserReloadtimer;
	float laserReloadtime;
	int maxBullets, lives;                                                   

public:
	PlayerManager();
	PlayerManager(Game *game);
	~PlayerManager();

	std::vector<sf::Sprite> &getShips();
	std::vector<Bullet> &getBullets();
	int getLives();
	sf::Vector2f getCenter();

	bool isPlayerDead();
	void removeLife();

	void movePlayerLeft(float secondsSinceLastFrame);
	void movePlayerRight(float secondsSinceLastFrame);
	void fire();

	void checkPlayerBounds();
	void checkBulletBounds();
	void moveBullets(float secondsSinceLastFrame);

	void addPlayerShip();

	void restart();

	void update(float secondsSinceLastFrame);
	void draw();
};

