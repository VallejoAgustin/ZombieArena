//ZombieArena.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

using namespace sf;

//handles w-a-s-d movement of the player
void wasdMovement(Player &);

int main(){
	enum class GameState {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	//game starts in game-over state
	GameState state = GameState::GAME_OVER;

	//find screen resolution
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	//create and render window
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	//create sfml view for main action
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;//how long playing state active

	//relative to world coords
	Vector2f mouseWorldPosition;
	//relative to screen coords
	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;//prev code (right click, outlining, hide selection

	while (window.isOpen()) {
		//handle events
		Event event;
		while(window.pollEvent(event)){
			if (event.type == Event::KeyPressed) {
				//pause
				if (event.key.code == Keyboard::Return && state == GameState::PLAYING) {
					state = GameState::PAUSED;
				}
				//unpause
				else if (event.key.code == Keyboard::Return && state == GameState::PLAYING) {
					state = GameState::PLAYING;

					clock.restart();//TODO:: comment out to see affect (reset clock so no frame jump)
				}
				//new-game from game over
				else if (event.key.code == Keyboard::Return && state == GameState::GAME_OVER) {
					state = GameState::LEVELING_UP;//????
				}

				if (state == GameState::PLAYING) {

				}
			}
		}

		//player quit
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (state == GameState::PLAYING) {
			wasdMovement(player);
		}
	}

    return 0;
}

void wasdMovement(Player & player) {
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		player.moveUp();
	}
	else {
		player.stopUp();
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		player.moveDown();
	}
	else {
		player.stopDown();
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		player.moveLeft();
	}
	else {
		player.stopLeft();
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		player.moveRight();
	}
	else {
		player.stopRight();
	}
}