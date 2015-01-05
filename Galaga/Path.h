#pragma once
#include "SFML\Graphics.hpp"
#include <math.h>

const double PI = 3.141592653589793238463;

class Path {
private:
	bool isCurve, shoot;

	float time, shotTime, turnSpeed, moveSpeed;
	sf::Clock timer;

	sf::Vector2f targetPosition; // used for moving directly to a location
public:
	Path();
	Path(float pathTime, float totalAngle, float pathSpeed, bool fire = false, float fireDelay = 0); // curved path
	Path(float pathSpeed, const sf::Vector2f &target, const sf::Vector2f &current, bool fire = false, float fireDelay = 0); // straight to target at pathSpeed
	Path(float pathTime, const sf::Vector2f &target, bool fire = false, float fireDelay = 0); // straight to target in pathTime
	~Path();

	void setShotTime(float t);

	void start();
	bool isDone();
	bool canShoot();

	float distance(const sf::Vector2f &a, const sf::Vector2f &b);
	void moveToNextPosition(sf::Sprite &sprite, float secondsSinceLastFrame); // modifies sprite to move it to next position
};

