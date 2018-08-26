#include "stdafx.h"
#include "Player.h"
#include "TextureHolder.h"

Player::Player() {
	_sprite = Sprite(TextureHolder::getTexture("graphics/player.png"));

	//initialize player variables
	_speed = START_SPEED;
	_health = START_HEALTH;
	_maxHealth = START_HEALTH;

	//center of sprite (for rotation)
	_sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	//place sprite in center of arena
	_position.x = arena.width / 2;
	_position.y = arena.height / 2;

	//copy arena to member variable arena
	_arena.left = arena.left;
	_arena.width = arena.width;
	_arena.top = arena.top;
	_arena.height = arena.height;

	_tileSize = tileSize;

	//store resolution
	_screenResolution.x = resolution.x;
	_screenResolution.y = resolution.y;
}

//reset player at end of game
void Player::resetPlayerStats() {
	_health = START_HEALTH;
	_maxHealth = START_HEALTH;
	_speed = START_SPEED;
}

bool Player::hit(Time timeHit) {
	//minimizes zombie damage to compensate for computer cycles
	if (timeHit.asMilliseconds() - _lastHit.asMilliseconds() > 200) {
		_lastHit = timeHit;
		_health -= 10;
		return true;
	}
	return false;
}

Time Player::lastHit() {
	return _lastHit;
}

FloatRect Player::getPosition() {
	return _sprite.getGlobalBounds();
}

Vector2f Player::getCenter() {
	return _position;
}

float Player::getRotation() {
	return _sprite.getRotation();
}

Sprite Player::getSprite() {
	return _sprite;
}

void Player::moveLeft() {
	_leftPressed = true;
}
void Player::moveRight() {
	_rightPressed = true;
}
void Player::moveUp() {
	_upPressed = true;
}
void Player::moveDown() {
	_downPressed = true;
}

void Player::stopLeft() {
	_leftPressed = false;
}
void Player::stopRight() {
	_rightPressed = false;
}
void Player::stopUp() {
	_upPressed = false;
}
void Player::stopDown() {
	_downPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition) {
	if (_upPressed) {
		_position.y -= _speed * elapsedTime;
	}
	if (_downPressed) {
		_position.y += _speed * elapsedTime;
	}
	if (_leftPressed) {
		_position.x -= _speed * elapsedTime;
	}
	if (_rightPressed) {
		_position.x += _speed * elapsedTime;
	}

	_sprite.setPosition(_position);

	//check if out-of-bounds
	if (_position.x > _arena.width - _tileSize) {
		_position.x = _arena.width - _tileSize;
	}
	if (_position.x < _arena.left + _tileSize) {
		_position.x = _arena.left + _tileSize;
	}
	if (_position.y > _arena.height - _tileSize) {
		_position.y = _arena.height - _tileSize;
	}
	if (_position.y < _arena.top + _tileSize) {
		_position.y = _arena.top + _tileSize;
	}

	//set rotation
	float angle = (atan2(mousePosition.y - _screenResolution.y / 2, mousePosition.x - _screenResolution.x / 2) * 180) / 3.141;

	_sprite.setRotation(angle);
}

void Player::upgradeSpeed() {
	//20% speed increase
	_speed += (START_SPEED * .2);
}

void Player::upgradeHealth() {
	//20% health increase
	_maxHealth += (START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount) {
	_health += amount;

	//ensure health does not exceed max
	if (_health > _maxHealth) {
		_health = _maxHealth;
	}
}

int Player::getHealth() {
	return _health;
}