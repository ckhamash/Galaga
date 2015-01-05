#include "Game.h"

Game::Game() 
	: window(sf::VideoMode(224 * scale, 324 * scale), "Galaga", sf::Style::Close)
	, playerManager(this)
	, enemyManager(this) {
	initText();
	loadHiScore();
}
	

Game::~Game() {}

void Game::initText() {
	font.loadFromFile("Fonts/PressStart2P.ttf");

	scoreText = sf::Text();
	scoreText.setFont(font);
	scoreText.setColor(sf::Color::Red);
	scoreText.setCharacterSize(8 * scale);
	scoreText.setString("SCORE");
	scoreText.setPosition(0, 0);

	scoreCounter = sf::Text();
	scoreCounter.setFont(font);
	scoreCounter.setColor(sf::Color::White);
	scoreCounter.setCharacterSize(8 * scale);
	scoreCounter.setPosition(0, scoreText.getCharacterSize());

	hiScoreText = sf::Text();
	hiScoreText.setFont(font);
	hiScoreText.setColor(sf::Color::Red);
	hiScoreText.setCharacterSize(8 * scale);
	hiScoreText.setString("HIGH SCORE");
	hiScoreText.setPosition(window.getSize().x - hiScoreText.getGlobalBounds().width + (1*scale), 0);

	hiScoreCounter = sf::Text();
	hiScoreCounter.setFont(font);
	hiScoreCounter.setColor(sf::Color::White);
	hiScoreCounter.setCharacterSize(8 * scale);

	livesText = sf::Text();
	livesText.setFont(font);
	livesText.setColor(sf::Color::White);
	livesText.setCharacterSize(8 * scale);
	livesText.setString("LIVES: " + std::to_string(playerManager.getLives()));
	livesText.setPosition(0, window.getSize().y - livesText.getCharacterSize() + (1*scale));

	levelText = sf::Text();
	levelText.setFont(font);
	levelText.setColor(sf::Color::Cyan);
	levelText.setCharacterSize(8 * scale);

	startText = sf::Text();
	startText.setFont(font);
	startText.setColor(sf::Color::Cyan);
	startText.setCharacterSize(8 * scale);
	startText.setString("PRESS SPACE TO START");
	startText.setPosition(window.getSize().x / 2 - startText.getGlobalBounds().width / 2, window.getSize().y / 2);

	restartText = sf::Text();
	restartText.setFont(font);
	restartText.setColor(sf::Color::Cyan);
	restartText.setCharacterSize(8 * scale);
	restartText.setString("PRESS SPACE TO RESTART");
	restartText.setPosition(window.getSize().x / 2 - restartText.getGlobalBounds().width / 2, window.getSize().y / 2);

	gameOverText = sf::Text();
	gameOverText.setFont(font);
	gameOverText.setColor(sf::Color::Red);
	gameOverText.setCharacterSize(8 * scale);
	gameOverText.setString("GAME OVER");
	gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - restartText.getCharacterSize());

	shipDestroyedText = sf::Text();
	shipDestroyedText.setFont(font);
	shipDestroyedText.setColor(sf::Color::Red);
	shipDestroyedText.setCharacterSize(8 * scale);
	shipDestroyedText.setString("SHIP DESTROYED");
	shipDestroyedText.setPosition(window.getSize().x / 2 - shipDestroyedText.getGlobalBounds().width / 2, window.getSize().y / 2 + restartText.getCharacterSize());
}

sf::RenderWindow& Game::getWindow() { return window; }

PlayerManager& Game::getPlayer() { return playerManager; }

int Game::getScale() { return scale; }

int Game::getLevel() { return level; }

void Game::addScore(int value) {
	score += value; 
	if (score > hiScore)
		hiScore = score;
}

void Game::loadHiScore() {
	std::ifstream saveFile;
	saveFile.open("hiscore.txt");
	if (saveFile.is_open()) {
		std::string line;
		std::getline(saveFile, line);
		hiScore = std::stoi(line);
	}
	else
		hiScore = 0;
	saveFile.close();
}

void Game::saveHiScore() {
	std::cout << "save\n";
	std::ofstream saveFile;
	saveFile.open("hiscore.txt");
	hiScore = score;
	saveFile << hiScore;
	saveFile.close();
}

void Game::checkInput() {
	sf::Event event;
	// inputs
	while (window.pollEvent(event)) { // check individual events
		switch (event.type) {
		case sf::Event::Closed: // window close
			window.close();
			break;
		case sf::Event::KeyPressed: // key press (not pressed -> pressed)
			switch (event.key.code) {
			case sf::Keyboard::Space:
				if (playerManager.getLives() < 1 || justStarted)
					restart();
				break;
			}
			break;
		case sf::Event::KeyReleased: // key release (pressed -> not pressed)
			break;
		}
	}
	// check state of individual keys (up or down)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && respawnTimer.getElapsedTime().asSeconds() > respawnTime)
		playerManager.fire();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		playerManager.movePlayerLeft(secondsSinceLastFrame);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		playerManager.movePlayerRight(secondsSinceLastFrame);
}

void Game::checkCollisions() {
	// check playerShips vs enemyShips
	if (respawnTimer.getElapsedTime().asSeconds() > respawnTime) {
		for (int i = 0; i < playerManager.getShips().size(); i++) {
			for (int j = 0; j < enemyManager.getShips().size(); j++)
				if (playerManager.getShips()[i].getGlobalBounds().intersects(enemyManager.getShips()[j].getSprite().getGlobalBounds())) {
					playerManager.getShips().erase(playerManager.getShips().begin() + i);
					enemyManager.getShips()[j].hit();
					i--;
					std::cout << playerManager.getShips().size() << std::endl;
					if (playerManager.isPlayerDead()) {
						if (playerManager.getLives() > 1) {
							playerManager.removeLife();
							respawn();
						}
						else {
							playerManager.removeLife();
							saveHiScore();
						}
					}
					
					break;
				}
		}
		// check playerShips vs enemyBullets
		for (int i = 0; i < playerManager.getShips().size(); i++) {
			for (int j = 0; j < enemyManager.getBullets().size(); j++)
				if (playerManager.getShips()[i].getGlobalBounds().intersects(enemyManager.getBullets()[j].getSprite().getGlobalBounds())) {
					playerManager.getShips().erase(playerManager.getShips().begin() + i);
					enemyManager.getBullets().erase(enemyManager.getBullets().begin() + j);
					i--;
					j--;
					if (playerManager.isPlayerDead()) {
						if (playerManager.getLives() > 1) {
							playerManager.removeLife();
							respawn();
						}
						else {
							playerManager.removeLife();
						}
					}
					break;
				}
		}
	}
	// check playerBullets vs enemyShips
	for (int i = 0; i < playerManager.getBullets().size(); i++) {
		for (int j = 0; j < enemyManager.getShips().size(); j++)
			if (playerManager.getBullets()[i].getSprite().getGlobalBounds().intersects(enemyManager.getShips()[j].getSprite().getGlobalBounds())) {
				playerManager.getBullets().erase(playerManager.getBullets().begin() + i);
				enemyManager.getShips()[j].hit();
				i--;
				break;
			}
	}
}

void Game::levelUp() {
	level++;
	levelTimer.restart();
}

void Game::respawn() {
	playerManager.addPlayerShip();
	respawnTimer.restart();
	std::cout << "respawn\n";
}

void Game::restart() {
	enemyManager.restart();
	playerManager.restart();
	levelTimer.restart();
	level = 1;
	score = 0;
	justStarted = false;
	std::cout << "restart\n";
}

void Game::run() {
	while (window.isOpen()) {
		//update
		update();
		//display
		draw();
	}
}

void Game::update() {
	secondsSinceLastFrame = frameTimer.restart().asSeconds(); // record frame's time step
	checkInput();

	// movement & bounds checking
	playerManager.update(secondsSinceLastFrame);
	enemyManager.update(secondsSinceLastFrame);

	// resolve collisions
	checkCollisions();
}

void Game::draw() {
	window.clear();
	// score message
	scoreCounter.setString(std::to_string(score));
	window.draw(scoreText);
	window.draw(scoreCounter);

	// hi score
	hiScoreCounter.setString(std::to_string(hiScore));
	hiScoreCounter.setPosition(window.getSize().x - hiScoreCounter.getGlobalBounds().width + (1 * scale), hiScoreText.getCharacterSize());
	window.draw(hiScoreText);
	window.draw(hiScoreCounter);

	// lives message
	livesText.setString("LIVES: " + std::to_string(playerManager.getLives()));
	window.draw(livesText);

	if (levelTimer.getElapsedTime().asSeconds() < levelStartTime && !justStarted) {
		// draw level message
		levelText.setString("LEVEL: " + std::to_string(level));
		levelText.setPosition(window.getSize().x / 2 - levelText.getGlobalBounds().width / 2, window.getSize().y / 2);
		window.draw(levelText);
	}
	if (justStarted) {
		// press space to start message
		window.draw(startText);
	}
	if (playerManager.getLives() < 1) {
		// game over message
		window.draw(gameOverText);
		// press space to restart message
		window.draw(restartText);
	}
		
	enemyManager.draw();
	if (respawnTimer.getElapsedTime().asSeconds() > respawnTime)
		playerManager.draw();
	else if (!justStarted) {
		// ship destroyed message
		window.draw(shipDestroyedText);
		if (std::cos(respawnTimer.getElapsedTime().asSeconds() * PI * 10) > 0)
			playerManager.draw();
	}

	window.display();
}