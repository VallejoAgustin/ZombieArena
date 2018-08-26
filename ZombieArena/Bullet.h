#ifndef BULLET_H
#define BULLET_H

#include "stdafx.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Bullet {
private:
	Vector2f _position;
	RectangleShape _bulletShape;

	bool inFlight = false;
	
	float _bulletSpeed = 1000;
	//x and y speed, derived from bullet speed
	float _bulletDistanceX, _bulletDistanceY;

	//boundaries to prevent infinite bullets
	float _maxX, _minX, _maxY, _minY;
public:
	Bullet();
	void stopBullet();
	bool isInFlight();
	void shoot(float, float, float, float);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(float);
};

#endif