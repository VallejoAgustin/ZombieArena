#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie {
private:
	//zombie speed
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//zombie health
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//variety
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	Vector2f _position;
	Sprite _sprite;
	float _speed;
	float _health;
	bool _alive;
public:
	bool hit();
	bool isAlive();
	void spawn(float, float, int, int);
	FloatRect getPosition();
	Sprite getSprite();
	void update(float, Vector2f);
};

#endif