#pragma once
#include "SFML\Graphics.hpp"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include <iostream>
#include <fstream>

class Game {
private:
	const int scale = 2;
	int level = 20;
	int score = 0, hiScore = 0;
	sf::RenderWindow window;
	PlayerManager playerManager;
	EnemyManager enemyManager;

	float secondsSinceLastFrame;
	float levelStartTime = 2.0f;
	float respawnTime = 2.0f;
	sf::Clock frameTimer, levelTimer, respawnTimer;

	bool justStarted = true;

	sf::Font font;

	sf::Text scoreText, scoreCounter, hiScoreText, hiScoreCounter, livesText, gameOverText, startText, restartText, levelText, shipDestroyedText;
public:
	Game();
	~Game();

	void initText();

	// getters
	sf::RenderWindow& getWindow();
	PlayerManager& getPlayer();
	int getScale();
	int getLevel();

	void loadHiScore();
	void saveHiScore();
	void addScore(int value);

	void checkInput();
	void checkCollisions();

	void levelUp();
	void respawn();
	void restart();

	void run();
	void update();
	void draw();
};