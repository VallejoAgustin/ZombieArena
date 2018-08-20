#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	Vector2f _position;
	Vector2f _screenResolution;
	Texture _texture;
	Sprite _sprite;
	IntRect _arena;
	int _tileSize;
	bool _upPressed, _downPressed, _leftPressed, _rightPressed;
	int _health;
	int _maxHealth;
	Time _lastHit;
	float _speed;
public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	
	//reset player at end of game
	void resetPlayerStats();

	bool hit(Time timeHit);
	Time lastHit();

	FloatRect getPosition();
	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void update(float elapsedTime, Vector2i mousePosition);

	void upgradeHealth();

	void increaseHealthLevel(int amount);

	int getHealth();
};