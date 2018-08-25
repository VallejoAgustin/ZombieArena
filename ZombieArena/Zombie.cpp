#include "stdafx.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

bool Zombie::hit() {
	_health--;

	if (_health < 0) {
		_alive = false;
		//load blood
		_sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));

		return true;
	}
	return false;
}
bool Zombie::isAlive() {
	return _alive;
}
void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch (type) {
	case 0://bloater
		_sprite = Sprite(TextureHolder::getTexture("graphics/bloater.png"));
		_speed = 40;
		_health = 5;
		break;
	case 1://chaser
		_sprite = Sprite(TextureHolder::getTexture("graphics/chaser.png"));
		_speed = 70;
		_health = 1;
		break;
	case 2://crawler
		_sprite = Sprite(TextureHolder::getTexture("graphics/crawler.png"));
		_speed = 20;
		_health = 3;
		break;
	default:
		//error
		break;
	}

	//seed random for p-random modifier
	srand((int)time(0) * seed);
	float modifier = rand() % MAX_VARIANCE + OFFSET;

	modifier /= 100;//between .7 and 1
	_speed *= modifier;

	//set position
	_position.x = startX;
	_position.y = startY;

	_sprite.setOrigin(25, 25);

	_sprite.setPosition(_position);
}
FloatRect Zombie::getPosition() {
	return _sprite.getGlobalBounds();
}
Sprite Zombie::getSprite() {
	return _sprite;
}
void Zombie::update(float elapsedTime, Vector2f playerLocation) {
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > _position.x) {//down
		_position.x = _position.x + _speed * elapsedTime;
	}

	if (playerY > _position.y) {//right
		_position.y = _position.y + _speed * elapsedTime;
	}

	if (playerX < _position.x) {//up
		_position.x = _position.x - _speed * elapsedTime;
	}

	if (playerY < _position.y) {//left
		_position.y = _position.y - _speed * elapsedTime;
	}

	_sprite.setPosition(_position);

	//rotate zombie depending on player location
	float angle = (atan2(playerY - _position.y, playerX - _position.x) * 180) / 3.141;
	_sprite.setRotation(angle);
}