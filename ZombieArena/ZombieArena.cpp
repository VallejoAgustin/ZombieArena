//ZombieArena.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <fstream>
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

	//about game
	int score = 0;
	int hiScore = 0;

	//home/game over screen
	Sprite spriteGameOver;
	Texture textureGameover = TextureHolder::getTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameover);
	spriteGameOver.setPosition(0, 0);

	//create view for hud
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::getTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(28, 620);

	//font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//TODO: create function for all these
	//paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(85);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(350, 200);
	pausedText.setString("Press Enter \nto continue");

	//gameover
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(280, 540);
	gameOverText.setString("Press Enter \nto play");

	//levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(60);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(80, 150);
	levelUpText.setString("1- Increased rate of fire\n2- Increased clip size(next reload)\n3- Increased max health\n4- Increased run speed\n5- More and better health pickups\n6- More and better ammo pickups");

	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(116, 620);

	//score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//load hiscore
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}

	//hi score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(80);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(980, 0);
	//std::stringstream ss;
	//ss << hiscore
	hiScoreText.setString("High score: " + hiScore);

	//zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(50);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(975, 620);
	//std::stringstream ss;
	//ss << hiscore
	zombiesRemainingText.setString("Zombies: 100");

	//wave
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(50);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(750, 620);
	//std::stringstream ss;
	//ss << hiscore
	zombiesRemainingText.setString("Wave: 0");

	//health
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(300, 620);

	//hud, to allow for efficiency
	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

	//handle sounds
	SoundBuffer hitBuffer, splatBuffer, shootBuffer, reloadBuffer, reloadFailedBuffer, powerUpBuffer, pickupBuffer;
	Sound hit, splat, shoot, reload, reloadFailed, powerUp, pickUp;
	hitBuffer.loadFromFile("sound/hit.wav");
	splatBuffer.loadFromFile("sound/splat.wav");
	shootBuffer.loadFromFile("sound/shoot.wav");
	reloadBuffer.loadFromFile("sound/reload.wav");
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	powerUpBuffer.loadFromFile("sound/powerup.wav");
	pickupBuffer.loadFromFile("sound/pickup.wav");
	hit.setBuffer(hitBuffer);
	splat.setBuffer(splatBuffer);
	shoot.setBuffer(shootBuffer);
	reload.setBuffer(reloadBuffer);
	reloadFailed.setBuffer(reloadFailedBuffer);
	powerUp.setBuffer(powerUpBuffer);
	pickUp.setBuffer(pickupBuffer);

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
					state = GameState::LEVELING_UP;

					//reset game
					wave = 0;
					score = 0;

					//TODO: func or class for this?
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					player.resetPlayerStats();
				}

				if (state == GameState::PLAYING) {
					//reloading
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							//enough ammo
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0) {
							//enough for SOME reload
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else {
							reloadFailed.play();
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

					//play sound
					shoot.play();

					bulletsInClip--;
				}
			}
		}

		//level
		if (state == GameState::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				//increase firerate
				fireRate++;
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				//increase clip size
				clipSize += clipSize;
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				//increase health
				player.upgradeHealth();
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				//increase speed
				player.upgradeSpeed();
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				//upgrade pickup
				healthPickup.upgrade();
				state = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				//upgrade pickup
				ammoPickup.upgrade();
				state = GameState::PLAYING;
			}

			if (state == GameState::PLAYING) {
				wave++;

				//will be modified
				arena.width = 500 * wave;
				arena.height = 500 * wave;
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
				numZombies = 5 * wave;

				//free up memory
				delete[] zombies;

				//use memory
				zombies = createHorde(numZombies, arena);

				numZombiesAlive = numZombies;

				//play powerup sound
				powerUp.play();

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

			//collision detection for bullet and zombies
			for (int i = 0; i < MAX_BULLETS; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						//if bullet intersects w/ active zombie
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							bullets[i].stopBullet();

							//register hit and check if dead (.hit())
							if (zombies[j].hit()) {
								score += 10;

								if (score >= hiScore) {
									hiScore = score;
								}

								numZombiesAlive--;
								//no more zombies
								if (numZombiesAlive == 0) {
									state = GameState::LEVELING_UP;
								}
							}
							//splat
							splat.play();
						}
					}
				}
			}

			//have zombies touched player
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					if (zombies[i].getPosition().intersects(player.getPosition())){
						if (player.hit(gameTimeTotal)) {
							hit.play();
						}

						//player died
						if (player.getHealth() <= 0) {
							state = GameState::GAME_OVER;

							//if new hiscore 
							if (score == hiScore) {
								std::ofstream outputFile("gamedata/scores.txt");
								outputFile << hiScore;
								outputFile.close();
							}
						}
					}
				}
			}

			//player touched pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				pickUp.play();
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				reload.play();
				bulletsSpare += ammoPickup.gotIt();
			}

			//size up healthbar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			framesSinceLastHUDUpdate++;
			//claculate fps
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				//set strings for hud
				ammoText.setString(bulletsInClip + "/" + bulletsSpare);
				scoreText.setString("Score: " + score);
				hiScoreText.setString("Hi-Score: " + hiScore);
				waveNumberText.setString("Wave: " + wave);
				zombiesRemainingText.setString("Zombies: " + numZombiesAlive);

				//reset
				framesSinceLastHUDUpdate = 0;
			}
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

			//switch hud view
			window.setView(hudView);

			//draw hud elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == GameState::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == GameState::PAUSED) {
			window.draw(pausedText);
		}
		if (state == GameState::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
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
