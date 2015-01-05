#include "PlayerManager.h"
#include "Game.h"

PlayerManager::PlayerManager() {}

PlayerManager::PlayerManager(Game *game)
	: parentGame(game) {
	playerMoveSpeed = 120 * game->getScale();
	bulletMoveSpeed = 300 * game->getScale();
	laserReloadtime = .01f;
	maxBullets = 2;
	lives = 3;
	shipTexture.loadFromFile("Images/PlayerShip.png");
	bulletTexture.loadFromFile("Images/PlayerBullet.png");
}

PlayerManager::~PlayerManager() {}

std::vector<sf::Sprite> &PlayerManager::getShips() { return playerShips; }

std::vector<Bullet> &PlayerManager::getBullets() { return playerBullets; }

int PlayerManager::getLives() { return lives; }

sf::Vector2f PlayerManager::getCenter() {
	int last = playerShips.size() - 1;
	if (last >= 0)
		return sf::Vector2f(playerShips[0].getGlobalBounds().left + playerShips[0].getGlobalBounds().width / 2, playerShips[0].getGlobalBounds().top);
	else
		sf::Vector2f();
}

bool PlayerManager::isPlayerDead() { return playerShips.size() < 1; }

void PlayerManager::removeLife() {lives--; }

void PlayerManager::movePlayerLeft(float secondsSinceLastFrame) {
	for (int i = 0; i < playerShips.size(); i++)
		playerShips[i].move(-playerMoveSpeed * secondsSinceLastFrame, 0);
}

void PlayerManager::movePlayerRight(float secondsSinceLastFrame) {
	for (int i = 0; i < playerShips.size(); i++)
		playerShips[i].move(playerMoveSpeed * secondsSinceLastFrame, 0);
}

void PlayerManager::fire() {
	if (laserReloadtimer.getElapsedTime().asSeconds() > laserReloadtime && playerBullets.size() < maxBullets * playerShips.size()) {
		for (int i = 0; i < playerShips.size(); i++) {
			sf::Vector2f shipNose(playerShips[i].getGlobalBounds().left + playerShips[i].getGlobalBounds().width / 2, playerShips[i].getGlobalBounds().top);
			playerBullets.push_back(Bullet(bulletTexture, sf::Vector2f(0, -bulletMoveSpeed)));
			int last = playerBullets.size() - 1;
			playerBullets[last].getSprite().scale(playerShips[i].getScale());
			playerBullets[last].getSprite().setPosition(shipNose.x - playerBullets[last].getSprite().getGlobalBounds().width / 2, shipNose.y);
		}
	}
	laserReloadtimer.restart();
}

void PlayerManager::checkPlayerBounds() {
	for (int i = 0; i < playerShips.size(); i++) {
		float difference = playerShips[i].getGlobalBounds().left;
		if (difference < 0)
			for (int j = 0; j < playerShips.size(); j++)
				playerShips[j].move(-difference, 0);
		difference = playerShips[i].getGlobalBounds().left + playerShips[i].getGlobalBounds().width - parentGame->getWindow().getSize().x;
		if (difference > 0)
			for (int j = 0; j < playerShips.size(); j++)
				playerShips[j].move(-difference, 0);
	}

}

void PlayerManager::checkBulletBounds() {
	for (int i = 0; i < playerBullets.size(); i++)
		if (!playerBullets[i].getSprite().getGlobalBounds().intersects(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(parentGame->getWindow().getSize())))) {
			playerBullets.erase(playerBullets.begin() + i);
			i--;
		}
}

void PlayerManager::moveBullets(float secondsSinceLastFrame) {
	for (int i = 0; i < playerBullets.size(); i++) {
		playerBullets[i].move(secondsSinceLastFrame);
	}
}

void PlayerManager::addPlayerShip() {
	playerShips.push_back(sf::Sprite(shipTexture));
	int last = playerShips.size() - 1;
	if (last > 0) {
		playerShips[last].scale(2, 2);
		playerShips[last].setPosition(sf::Vector2f(playerShips[last - 1].getGlobalBounds().left + playerShips[last - 1].getGlobalBounds().width, parentGame->getWindow().getSize().y - playerShips[last].getGlobalBounds().height * 2));
	}
	else {
		last = 0;
		playerShips[last].scale(parentGame->getScale(), parentGame->getScale());
		playerShips[last].setPosition(parentGame->getWindow().getSize().x / 2, parentGame->getWindow().getSize().y - playerShips[0].getGlobalBounds().height * 2);
	}
}

void PlayerManager::restart() {
	lives = 3;
	playerBullets.clear();
	playerShips.clear();
	laserReloadtimer.restart();
	addPlayerShip();
}

void PlayerManager::update(float secondsSinceLastFrame) {
	moveBullets(secondsSinceLastFrame);
	checkPlayerBounds();
	checkBulletBounds();
}

void PlayerManager::draw() {
	for (int i = 0; i < playerBullets.size(); i++)
		parentGame->getWindow().draw(playerBullets[i].getSprite());
	for (int i = 0; i < playerShips.size(); i++)
		parentGame->getWindow().draw(playerShips[i]);
}