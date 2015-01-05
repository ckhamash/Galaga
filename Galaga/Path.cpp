#include "Path.h"

Path::Path() {}

Path::Path(float pathTime, float totalAngle, float pathSpeed, bool fire, float fireDelay) 
	: time(pathTime)
	, moveSpeed(pathSpeed)
	, turnSpeed(totalAngle / pathTime)
	, isCurve(true)
	, shoot(fire) { 
}

Path::Path(float pathSpeed, const sf::Vector2f &target, const sf::Vector2f &current, bool fire, float fireDelay)
	: targetPosition(target)
	, time(distance(target, current) / pathSpeed)
	, isCurve(false)
	, shoot(fire) {
}

Path::Path(float pathTime, const sf::Vector2f &target, bool fire, float fireDelay)
	: targetPosition(target)
	, time(pathTime)
	, isCurve(false)
	, shoot(fire) {
}

Path::~Path() {}

void Path::setShotTime(float t) { shotTime = t; }

void Path::start() { timer.restart(); }

bool Path::isDone() { return time < timer.getElapsedTime().asSeconds(); }

bool Path::canShoot() { return shotTime < timer.getElapsedTime().asSeconds() && shoot; }

float Path::distance(const sf::Vector2f &a, const sf::Vector2f &b) {
	sf::Vector2f difference = a - b;
	return std::sqrt(difference.x * difference.x + difference.y * difference.y);
}

void Path::moveToNextPosition(sf::Sprite &sprite, float secondsSinceLastFrame) { 
	if (time == 0)
		sprite.setPosition(targetPosition);
	else if (isCurve) {
		if (time > timer.getElapsedTime().asSeconds()) {
			sprite.rotate(turnSpeed * secondsSinceLastFrame);
			float rotation = sprite.getRotation();
			sf::Vector2f move(std::sin(rotation * PI / 180.0f), -std::cos(rotation * PI / 180.0f));
			move *= moveSpeed * secondsSinceLastFrame;
			sprite.move(move);
		}
	}
	else {
		float timeLeft = time - timer.getElapsedTime().asSeconds();
		float speed = 0;
		if (timeLeft > 0) {
			speed = distance(targetPosition, sprite.getPosition()) / timeLeft;
			sf::Vector2f difference = targetPosition - sprite.getPosition();
			if (difference.y < 0)
				sprite.setRotation((std::atan(-difference.x / difference.y) * 180.0f / PI));
			else
				sprite.setRotation((std::atan(-difference.x / difference.y) * 180.0f / PI + 180.0f));
			sprite.move((difference / timeLeft) * secondsSinceLastFrame);
		}
		else {
			sprite.setRotation(0);
		}
	}
}