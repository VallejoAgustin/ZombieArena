#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(){
	_bulletShape.setSize(sf::Vector2f(2, 2));
}
void Bullet::stopBullet(){
	inFlight = false;
}
bool Bullet::isInFlight(){
	return inFlight;
}
void Bullet::shoot(float startX, float startY, float targetX, float targetY){
	inFlight = true;
	_position.x = startX;
	_position.y = startY;

	//slope?
	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0) {
		gradient *= -1;
	}

	float ratioXY = _bulletSpeed / (1 + gradient);

	_bulletDistanceY = ratioXY;
	_bulletDistanceX = ratioXY * gradient;

	//point bullet in correct direction
	if (targetX < startX) {
		_bulletDistanceX *= -1;
	}
	if (targetY < startY) {
		_bulletDistanceY *= -1;
	}

	//max range of 1k pixels
	float range = 1000;
	_minX = startX - range;
	_maxX = startX + range;
	_minY = startY - range;
	_maxY = startY + range;

	_bulletShape.setPosition(_position);
}
FloatRect Bullet::getPosition(){
	return _bulletShape.getGlobalBounds();
}
RectangleShape Bullet::getShape(){
	return _bulletShape;
}
void Bullet::update(float elapsedTime){
	_position.x += _bulletDistanceX * elapsedTime;
	_position.y += _bulletDistanceY * elapsedTime;

	_bulletShape.setPosition(_position);

	//out of range?
	if (_position.x < _minX || _position.x > _maxX || _position.y < _minY || _position.y > _maxY) {
		inFlight = false;
	}
}