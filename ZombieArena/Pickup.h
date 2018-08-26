#ifndef PICKUP_H
#define PICKUP_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup {
private:
	//start value for pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	Sprite _sprite;
	IntRect _arena;

	//pickup worth
	int _value;
	//type of pickup 1 = health, 2 = ammo //TODO: use enumeration
	int _type;

	bool _spawned;
	float _secondsSinceSpawn, _secondsSinceDeSpawn, _secondsToLive, _secondsToWait;
public:
	Pickup(int);
	void setArena(IntRect);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float);
	bool isSpawned();
	int gotIt();
	void upgrade();
};

#endif