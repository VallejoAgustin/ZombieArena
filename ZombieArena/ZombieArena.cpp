//ZombieArena.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "CreateBackground.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;

enum class GameState {
	PAUSED, LEVELING_UP, GAME_OVER, PLAYING
};

//handles w-a-s-d movement of the player
void wasdMovement(Player &);

int main(){
	TextureHolder holder;
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

	//create background
	VertexArray background;
	Texture textureBackground = TextureHolder::getTexture("graphics/background_sheet.png");

	int numZombies, numZombiesAlive;
	Zombie* zombies = nullptr;

	//setting constant bullet array (dont expect player to exceed 100 bullets)
	const int MAX_BULLETS = 100;
	Bullet bullets[MAX_BULLETS];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time fireLastPressed;

	//hide mouse pointer and replace w/ crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::getTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//create pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

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
					//reloading
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							//enough ammo
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0) {
							//enough for SOME reload
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else {
							//TODO:sound
						}
					}
				}
			}
		}

		//player quit
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//movement
		if (state == GameState::PLAYING) {
			wasdMovement(player);

			//fire bullet
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - fireLastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {
					//shoot bullet, increase bullets active
					bullets[currentBullet++].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

					//over the '100' array
					if (currentBullet > MAX_BULLETS - 1) {
						currentBullet = 0;
					}

					fireLastPressed = gameTimeTotal;

					bulletsInClip--;
				}
			}
		}

		//level
		if (state == GameState::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				state = GameState::PLAYING;
			}

			if (state == GameState::PLAYING) {
				//will be modified
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				//pass vertex array by reference
				int tileSize = createBackground(background, arena);

				//spawn player in middle of arena
				player.spawn(arena, resolution, tileSize);

				//configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//create horde
				numZombies = 10;//TODO: dynamic on level

				//free up memory
				delete[] zombies;

				//use memory
				zombies = createHorde(numZombies, arena);

				numZombiesAlive = numZombies;

				clock.restart();
			}
		}

		//update frame

		if (state == GameState::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();

			//find mouse pointer
			mouseScreenPosition = Mouse::getPosition();
			//convert to world coords
			mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition, mainView);
			
			//set crosshair to mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);
			
			//update player based on mouse::getposi();
			player.update(dtAsSeconds, mouseScreenPosition);

			//save player's new position
			Vector2f playerPosition(player.getCenter());

			//view center around player
			mainView.setCenter(player.getCenter());

			//update all zombies that are alive TODO: list
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//update bullets in flight
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}

			//update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
		}

		//draw scene
		if (state == GameState::PLAYING) {
			window.clear();

			//set window view to main view
			window.setView(mainView);

			//draw background
			window.draw(background, &textureBackground);

			//draw zombies, dead ones too for splatter image (blood)
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			//draw bullets if active
			for (int i = 0; i < MAX_BULLETS; i++) {
				if(bullets[i].isInFlight()){
					window.draw(bullets[i].getShape());
				}
			}

			//draw player
			window.draw(player.getSprite());

			//draw pickups
			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}

			//draw cursor
			window.draw(spriteCrosshair);
		}

		if (state == GameState::LEVELING_UP) {

		}
		if (state == GameState::PAUSED) {

		}
		if (state == GameState::GAME_OVER) {

		}

		window.display();
	}

	//free memory
	delete[] zombies;
	zombies = nullptr;

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
		player.moveLeft();
	}
	else {
		player.stopLeft();
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		player.moveDown();
	}
	else {
		player.stopDown();
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		player.moveRight();
	}
	else {
		player.stopRight();
	}
}
