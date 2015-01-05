#pragma once
#include "SFML\Graphics.hpp"
#include "Enemy.h"
#include <random>

class Game;

class EnemyManager {
	Game *parentGame;

	sf::Texture droneTexture, bomberTexture, commanderTexture, damagedCommanderTexture, bulletTexture;

	float shotChance, maxChance, attackChance;
	int maxShots, maxAttackers;

	std::vector<Enemy> enemyShips;
	std::vector<Bullet> enemyBullets;
	float enemyMoveSpeed, bulletMoveSpeed;

	std::vector<Enemy> currentWave;
	std::vector<std::vector<Path>> spawnPaths; // list of possible spawn paths
	std::vector<sf::Vector2f> startPositions; // and respective start positions
	std::vector<float> startRotations; // and respective orientations

	std::vector<sf::Vector2f> formationPositions;
	std::vector<int> usedFormationPositions;

	std::vector<std::vector<Path>> attackPaths;

	float spawnTime, waveTime, attackTime;
	sf::Clock spawnTimer;

	std::random_device randomEngine;
	std::uniform_real_distribution<float> percentDistribution;
public:
	EnemyManager();
	EnemyManager(Game *game);
	~EnemyManager();

	void initFormation();
	void initSpawnPaths();
	void initAttackPaths();

	std::vector<Enemy> &getShips();
	std::vector<Bullet> &getBullets();

	void createWave(int size);
	void spawnWave();
	void sendAttacker();

	Bullet createBullet(sf::Vector2f position, sf::Vector2f target);

	void checkBulletBounds();
	void removeDead();
	void moveBullets(float secondsSinceLastFrame);
	void moveShips(float secondsSinceLastFrame);

	void restart();
	void update(float secondsSinceLastFrame);
	void draw();

	std::vector<Path> spawnSideA();
	std::vector<Path> spawnSideB();
	std::vector<Path> spawnTopA();
	std::vector<Path> spawnTopB();

	std::vector<Path> attackBottomA();
	std::vector<Path> attackBottomB();
	std::vector<Path> attackMiddleA();
	std::vector<Path> attackMiddleB();
	std::vector<Path> attackTopA();
	std::vector<Path> attackTopB();
};

