#include "stdafx.h"
#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type){
	_type = type;

	if (_type == 1) {
		_sprite = Sprite(TextureHolder::getTexture("graphics/health_pickup.png"));
		_value = HEALTH_START_VALUE;
	}
	else {//_type == 2
		_sprite = Sprite(TextureHolder::getTexture("graphics/ammo_pickup.png"));
		_value = AMMO_START_VALUE;
	}

	_sprite.setOrigin(25, 25);

	_secondsToLive = START_SECONDS_TO_LIVE;
	_secondsToWait = START_WAIT_TIME;
}
void Pickup::setArena(IntRect arena){
	//modified 'arena' space due to spawning being in-bounds
	_arena.height = arena.height - 50;
	_arena.left = arena.left + 50;
	_arena.top = arena.top + 50;
	_arena.width = arena.width - 50;

	spawn();
}
void Pickup::spawn(){
	//random x and y positions
	srand((int)time(0) / _type);
	int x = rand() % _arena.width;
	srand((int)time(0) * _type);
	int y = rand() % _arena.height;

	_secondsSinceSpawn = 0;
	_spawned = true;

	_sprite.setPosition(x, y);
}
FloatRect Pickup::getPosition(){
	return _sprite.getGlobalBounds;
}
Sprite Pickup::getSprite(){
	return _sprite;
}
void Pickup::update(float elapsedTime){
	if (_spawned) {
		_secondsSinceSpawn += elapsedTime;
	}
	else {
		_secondsSinceDeSpawn += elapsedTime;
	}

	//despawn?
	if (_secondsSinceSpawn > _secondsToLive && _spawned) {
		_spawned = false;
		_secondsSinceDeSpawn = 0;
	}

	//spawn?
	if (_secondsSinceDeSpawn > _secondsToWait && !_spawned) {
		spawn();
	}
}
bool Pickup::isSpawned(){
	return _spawned;
}
int Pickup::gotIt(){
	_spawned = false;
	_secondsSinceDeSpawn = 0;

	return _value;
}
void Pickup::upgrade(){
	if (_type == 1) {
		_value += (HEALTH_START_VALUE * .5);
	}
	else {
		_value += (AMMO_START_VALUE * .5);
	}

	//make more frequent and stay spawned for longer
	_secondsToLive += (START_SECONDS_TO_LIVE / 10);
	_secondsToWait -= (START_WAIT_TIME / 10);
}