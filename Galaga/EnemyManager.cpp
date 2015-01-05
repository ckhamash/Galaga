#include "EnemyManager.h"
#include "Game.h"

EnemyManager::EnemyManager() {}

EnemyManager::EnemyManager(Game *game) 
	: parentGame(game) {
	percentDistribution = std::uniform_real_distribution<float>(0, 1);
	enemyMoveSpeed = 150 * game->getScale();
	bulletMoveSpeed = 150 * game->getScale();
	droneTexture.loadFromFile("Images/EnemyDrone.png");
	bomberTexture.loadFromFile("Images/EnemyBomber.png");
	commanderTexture.loadFromFile("Images/EnemyCommander.png");
	damagedCommanderTexture.loadFromFile("Images/EnemyCommanderDamaged.png");
	bulletTexture.loadFromFile("Images/EnemyBullet.png");
	shotChance = 0.02f;
	attackChance = 0.02f;
	maxChance = 0.75f;
	maxShots = 3;
	maxAttackers = 6;

	spawnTime = .15f;
	waveTime = 2.0f;
	attackTime = .1f;

	initFormation();
	initSpawnPaths();
	initAttackPaths();
}

EnemyManager::~EnemyManager() {}

std::vector<Enemy> &EnemyManager::getShips() { return enemyShips; }

std::vector<Bullet> &EnemyManager::getBullets() { return enemyBullets; }

void EnemyManager::initFormation() {
	float mid = parentGame->getWindow().getSize().x / 2;
	float top = commanderTexture.getSize().y * parentGame->getScale() * 2;
	float horiz = (commanderTexture.getSize().x  + 2) * parentGame->getScale();
	float vert = (commanderTexture.getSize().y + 2) * parentGame->getScale();
	formationPositions = {
		sf::Vector2f(mid - horiz * 1.5, top), sf::Vector2f(mid - horiz * 0.5, top), sf::Vector2f(mid + horiz * 0.5, top), sf::Vector2f(mid + horiz * 1.5, top),
		sf::Vector2f(mid - horiz * 3.5, top + vert), sf::Vector2f(mid - horiz * 2.5, top + vert), sf::Vector2f(mid - horiz * 1.5, top + vert), sf::Vector2f(mid - horiz * 0.5, top + vert),
		sf::Vector2f(mid + horiz * 3.5, top + vert), sf::Vector2f(mid + horiz * 2.5, top + vert), sf::Vector2f(mid + horiz * 1.5, top + vert), sf::Vector2f(mid + horiz * 0.5, top + vert),
		sf::Vector2f(mid - horiz * 3.5, top + vert * 2), sf::Vector2f(mid - horiz * 2.5, top + vert * 2), sf::Vector2f(mid - horiz * 1.5, top + vert * 2), sf::Vector2f(mid - horiz * 0.5, top + vert * 2),
		sf::Vector2f(mid + horiz * 3.5, top + vert * 2), sf::Vector2f(mid + horiz * 2.5, top + vert * 2), sf::Vector2f(mid + horiz * 1.5, top + vert * 2), sf::Vector2f(mid + horiz * 0.5, top + vert * 2),
		sf::Vector2f(mid - horiz * 3.5, top + vert * 3), sf::Vector2f(mid - horiz * 2.5, top + vert * 3), sf::Vector2f(mid - horiz * 1.5, top + vert * 3), sf::Vector2f(mid - horiz * 0.5, top + vert * 3),
		sf::Vector2f(mid + horiz * 3.5, top + vert * 3), sf::Vector2f(mid + horiz * 2.5, top + vert * 3), sf::Vector2f(mid + horiz * 1.5, top + vert * 3), sf::Vector2f(mid + horiz * 0.5, top + vert * 3),
		sf::Vector2f(mid - horiz * 3.5, top + vert * 4), sf::Vector2f(mid - horiz * 2.5, top + vert * 4), sf::Vector2f(mid - horiz * 1.5, top + vert * 4), sf::Vector2f(mid - horiz * 0.5, top + vert * 4),
		sf::Vector2f(mid + horiz * 3.5, top + vert * 4), sf::Vector2f(mid + horiz * 2.5, top + vert * 4), sf::Vector2f(mid + horiz * 1.5, top + vert * 4), sf::Vector2f(mid + horiz * 0.5, top + vert * 4),
		sf::Vector2f(mid - horiz * 4.5, top + vert * 4), sf::Vector2f(mid - horiz * 4.5, top + vert * 3), sf::Vector2f(mid + horiz * 4.5, top + vert * 3), sf::Vector2f(mid + horiz * 4.5, top + vert * 4), };
}

void EnemyManager::initSpawnPaths() {
	spawnPaths.push_back(spawnSideA());
	startPositions.push_back(sf::Vector2f(0, parentGame->getWindow().getSize().y * (3.0f / 4.0f)));
	startRotations.push_back(90);
	spawnPaths.push_back(spawnSideB());
	startPositions.push_back(sf::Vector2f(parentGame->getWindow().getSize().x, parentGame->getWindow().getSize().y * (3.0f / 4.0f)));
	startRotations.push_back(270);
	spawnPaths.push_back(spawnTopA());
	startPositions.push_back(sf::Vector2f(parentGame->getWindow().getSize().x * (1.0f / 3.0f), 0));
	startRotations.push_back(180);
	spawnPaths.push_back(spawnTopB());
	startPositions.push_back(sf::Vector2f(parentGame->getWindow().getSize().x * (2.0f / 3.0f), 0));
	startRotations.push_back(180);
}

void EnemyManager::initAttackPaths() {
	attackPaths.push_back(attackBottomA());
	attackPaths.push_back(attackBottomB());
	attackPaths.push_back(attackMiddleA());
	attackPaths.push_back(attackMiddleB());
	attackPaths.push_back(attackTopA());
	attackPaths.push_back(attackTopB());
}

void EnemyManager::createWave(int size) {
	if (size > formationPositions.size() - usedFormationPositions.size())
		size = formationPositions.size() - usedFormationPositions.size();
	int index;
	index = percentDistribution(randomEngine) * (float)spawnPaths.size();
	sf::Vector2f waveStart;
	float waveRotation;
	std::vector<Path> wavePath;
	// create start position for wave
	waveStart = startPositions[index];
	// create start orientation for wave
	waveRotation = startRotations[index];
	// create the path the wave will use
	wavePath = spawnPaths[index];

	// create the enemies
	for (int i = 0; i < size; i++) {
		std::vector<Path> enemyPath = wavePath;
		bool empty = false;
		while (!empty) { // find a location in the formation
			index = percentDistribution(randomEngine) * (float)formationPositions.size();
			if (usedFormationPositions.size() == 0) {
				empty = true;
				break;
			}
			empty = true;
			for (int j = 0; j < usedFormationPositions.size(); j++) {
				if (index == usedFormationPositions[j]) {
					empty = false;
					break;
				}
			}
		}
		usedFormationPositions.push_back(index);
		// select type of enemy based on formation position
		sf::Texture *texture;
		int score = 0;
		int health = 1;
		if (0 <= index && index <= 3) {
			texture = &commanderTexture;
			score = 400;
			health = 2;
		}
		else if (4 <= index && index <= 19) {
			texture = &bomberTexture;
			score = 80;
		}
		else {
			texture = &droneTexture;
			score = 50;
		}

		// add final location to this enemy's path
		enemyPath.push_back(Path(0.5f, formationPositions[index]));
		// add enemy to wave
		Enemy enemy(*texture, enemyPath, formationPositions[index], score, health);
		
		float chance = std::min(shotChance * parentGame->getLevel(), maxChance);
		for (int i = 0; i < maxShots; i++) {
			if (percentDistribution(randomEngine) < chance) {
				enemy.shotsLeft()++;
			}
			else {
				break;
			}
		}
		enemy.getSprite().scale(parentGame->getScale(), parentGame->getScale());
		enemy.getSprite().setOrigin(enemy.getSprite().getLocalBounds().width / 2, enemy.getSprite().getLocalBounds().height / 2);
		enemy.getSprite().setPosition(waveStart);
		enemy.getSprite().setRotation(waveRotation);
		currentWave.push_back(enemy);
	}
	spawnTimer.restart();
}

void EnemyManager::spawnWave() {
	if (spawnTime < spawnTimer.getElapsedTime().asSeconds()) {
		currentWave[0].start();
		enemyShips.push_back(currentWave[0]);
		currentWave.erase(currentWave.begin());
		spawnTimer.restart();
	}
}

void EnemyManager::sendAttacker() {
	// randomly picks an enemy in formation
	if (attackTime < spawnTimer.getElapsedTime().asSeconds() && enemyShips.size() > 0) {
		int index = -1;
		int notInFormation = 0;
		for (int i = 0; i < enemyShips.size(); i++)
			if (!enemyShips[i].pathsFinished() && enemyShips[i].getSprite().getPosition() != enemyShips[i].getPosition()) {
				notInFormation++;
			}
		float chance = std::min(attackChance * parentGame->getLevel(), maxChance);
		while (notInFormation < maxAttackers && percentDistribution(randomEngine) < chance) {
			index = percentDistribution(randomEngine) * (float)enemyShips.size();
			if (enemyShips[index].pathsFinished() && enemyShips[index].getSprite().getPosition() == enemyShips[index].getPosition())
				break;
		}
		// give it a new attack path
		if (index > 0 && enemyShips[index].getSprite().getPosition() == enemyShips[index].getPosition()) {
			int pathIndex;
			int shotMultiplier = 1;
			if (enemyShips[index].getSprite().getTexture() == &droneTexture) {
				pathIndex = 0 + percentDistribution(randomEngine) * 2;
			}
			else if (enemyShips[index].getSprite().getTexture() == &bomberTexture) {
				pathIndex = 2 + percentDistribution(randomEngine) * 2;
				shotMultiplier = 2;
			}
			else if (enemyShips[index].getSprite().getTexture() == &commanderTexture || enemyShips[index].getSprite().getTexture() == &damagedCommanderTexture) {
				pathIndex = 4 + percentDistribution(randomEngine) * 2;
				shotMultiplier = 3;
			}
			std::vector<Path> attackPath = attackPaths[pathIndex];
			attackPath.push_back(Path(0, enemyShips[index].getPosition() - sf::Vector2f(0, commanderTexture.getSize().y * parentGame->getScale() * 8)));
			attackPath.push_back(Path(2.0f, enemyShips[index].getPosition()));
			enemyShips[index].setPaths(attackPath);
			enemyShips[index].start();

			float chance = std::min(shotChance * parentGame->getLevel() * shotMultiplier, maxChance);
			enemyShips[index].shotsLeft() = 0;
			for (int i = 0; i < maxShots * shotMultiplier; i++) {
				if (percentDistribution(randomEngine) < chance) {
					enemyShips[index].shotsLeft()++;
				}
				else {
					break;
				}
			}
		}
		spawnTimer.restart();
	}
}

Bullet EnemyManager::createBullet(sf::Vector2f position, sf::Vector2f target) {
	float time = std::abs(target.y - position.y) / bulletMoveSpeed;
	float xVel = (target.x - position.x) / time;
	return Bullet(bulletTexture, sf::Vector2f(xVel, bulletMoveSpeed));
}

void EnemyManager::checkBulletBounds() {
	for (int i = 0; i < enemyBullets.size(); i++)
		if (!enemyBullets[i].getSprite().getGlobalBounds().intersects(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(parentGame->getWindow().getSize())))) {
			enemyBullets.erase(enemyBullets.begin() + i);
			i--;
		}
}

void EnemyManager::removeDead() {
	for (int i = 0; i < enemyShips.size(); i++) {
		if (enemyShips[i].isDead()) {
			parentGame->addScore(enemyShips[i].getValue());
			enemyShips.erase(enemyShips.begin() + i);
			i--;
		}
		else if (enemyShips[i].getSprite().getTexture() == &commanderTexture && enemyShips[i].getHealth() < 2)
			enemyShips[i].getSprite().setTexture(damagedCommanderTexture);
	}
}

void EnemyManager::moveBullets(float secondsSinceLastFrame) {
	for (int i = 0; i < enemyBullets.size(); i++) {
		enemyBullets[i].move(secondsSinceLastFrame);
	}
}

void EnemyManager::moveShips(float secondsSinceLastFrame) {
	for (int i = 0; i < enemyShips.size(); i++) {
		enemyShips[i].move(secondsSinceLastFrame);
		if (enemyShips[i].canShoot() && parentGame->getPlayer().getShips().size() > 0) {
			enemyShips[i].fire();
			Bullet bullet = createBullet(enemyShips[i].getSprite().getPosition(), parentGame->getPlayer().getCenter());
			bullet.getSprite().setScale(parentGame->getScale(), parentGame->getScale());
			bullet.getSprite().setPosition(enemyShips[i].getSprite().getPosition());
			enemyBullets.push_back(bullet);
		}
	}
}

void EnemyManager::restart() {
	enemyBullets.clear();
	enemyShips.clear();
	usedFormationPositions.clear();
	currentWave.clear();
	spawnTimer.restart();
}

void EnemyManager::update(float secondsSinceLastFrame) {
	moveBullets(secondsSinceLastFrame);
	moveShips(secondsSinceLastFrame);
	checkBulletBounds();
	if (!parentGame->getPlayer().isPlayerDead()) {
		if (currentWave.size() > 0)
			spawnWave();
		else if (usedFormationPositions.size() < formationPositions.size()) {
			if (waveTime < spawnTimer.getElapsedTime().asSeconds())
				createWave(formationPositions.size() / 5);
		}
		else if (enemyShips.size() < 1) {
			parentGame->levelUp();
			usedFormationPositions.clear();
		}
		else {
			sendAttacker();
		}
	}
	removeDead();
}

void EnemyManager::draw() {
	for (int i = 0; i < enemyBullets.size(); i++)
		parentGame->getWindow().draw(enemyBullets[i].getSprite());
	for (int i = 0; i < enemyShips.size(); i++)
		parentGame->getWindow().draw(enemyShips[i].getSprite());
}

std::vector<Path> EnemyManager::spawnSideA() {
	std::vector<Path> path;
	path.push_back(Path(0.65f, -45, enemyMoveSpeed));
	path.push_back(Path(1.0f, -360, enemyMoveSpeed, true, 0.5f));
	path.push_back(Path(0.5f, -45, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::spawnSideB() {
	std::vector<Path> path;
	path.push_back(Path(0.65f, 45, enemyMoveSpeed));
	path.push_back(Path(1.0f, 360, enemyMoveSpeed, true, 0.5f));
	path.push_back(Path(0.5f, 45, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::spawnTopA() {
	std::vector<Path> path;
	path.push_back(Path(0.9f, -80, enemyMoveSpeed, true, 0.3f));
	path.push_back(Path(0.8f, 170, enemyMoveSpeed));
	path.push_back(Path(0.25f, 90, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::spawnTopB() {
	std::vector<Path> path;
	path.push_back(Path(0.9f, 80, enemyMoveSpeed, true, 0.3f));
	path.push_back(Path(0.8f, -170, enemyMoveSpeed));
	path.push_back(Path(0.25f, -90, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackBottomA() {
	std::vector<Path> path;
	path.push_back(Path(0.1f, -180, enemyMoveSpeed));
	path.push_back(Path(0.4f, -60, enemyMoveSpeed, true));
	path.push_back(Path(0.8f, 120, enemyMoveSpeed));
	path.push_back(Path(0.4f, -60, enemyMoveSpeed));
	path.push_back(Path(1.6f, -360, enemyMoveSpeed));
	path.push_back(Path(0.4f, -60, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackBottomB() { 
	std::vector<Path> path;
	path.push_back(Path(0.1f, 180, enemyMoveSpeed));
	path.push_back(Path(0.4f, 60, enemyMoveSpeed, true));
	path.push_back(Path(0.8f, -120, enemyMoveSpeed));
	path.push_back(Path(0.4f, 60, enemyMoveSpeed));
	path.push_back(Path(1.6f, 360, enemyMoveSpeed));
	path.push_back(Path(0.4f, 60, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackMiddleA() {
	std::vector<Path> path;
	path.push_back(Path(0.1f, -180, enemyMoveSpeed));
	path.push_back(Path(0.4f, -80, enemyMoveSpeed, true, 0.2f));
	path.push_back(Path(0.1f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.8f, 160, enemyMoveSpeed, true));
	path.push_back(Path(0.2f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.5f, -100, enemyMoveSpeed));
	path.push_back(Path(0.8f, 60, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackMiddleB() {
	std::vector<Path> path;
	path.push_back(Path(0.1f, 180, enemyMoveSpeed));
	path.push_back(Path(0.4f, 80, enemyMoveSpeed, true, 0.2f));
	path.push_back(Path(0.1f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.8f, -160, enemyMoveSpeed, true));
	path.push_back(Path(0.2f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.5f, 100, enemyMoveSpeed));
	path.push_back(Path(0.8f, -60, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackTopA() {
	std::vector<Path> path;
	path.push_back(Path(0.1f, -180, enemyMoveSpeed));
	path.push_back(Path(0.8f, -60, enemyMoveSpeed, true, 0.2f));
	path.push_back(Path(0.6f, 180, enemyMoveSpeed, true));
	path.push_back(Path(0.4f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.6f, 180, enemyMoveSpeed, true));
	path.push_back(Path(1.0f, 80, enemyMoveSpeed));
	path.push_back(Path(0.8f, -60, enemyMoveSpeed));
	return path;
}

std::vector<Path> EnemyManager::attackTopB() {
	std::vector<Path> path;
	path.push_back(Path(0.1f, 180, enemyMoveSpeed));
	path.push_back(Path(0.8f, 60, enemyMoveSpeed, true, 0.2f));
	path.push_back(Path(0.6f, -180, enemyMoveSpeed, true));
	path.push_back(Path(0.4f, 0, enemyMoveSpeed, true));
	path.push_back(Path(0.6f, -180, enemyMoveSpeed, true));
	path.push_back(Path(1.0f, -80, enemyMoveSpeed));
	path.push_back(Path(0.8f, 60, enemyMoveSpeed));
	return path;
}