#include "stdafx.h"
#include "CreateBackground.h"
#include "Zombie.h"

Zombie* createHorde(int numZombies, IntRect arena) {
	//TODO: use list to increase 
	Zombie* zombies = new Zombie[numZombies];//dynamic array

	int maxY = arena.height - 20;//+/- 20 to prevent zombies appearing through walls
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numZombies; i++) {
		srand((int)time(0) * i);
		int side = rand() % 4;
		float x, y;

		switch (side) {
		case 0://left
			x = minX;
			y = rand() % maxY + minY;
			break;
		case 1://right
			x = maxX;
			y = rand() % maxY + minY;
			break;
		case 2://top
			x = rand() % maxX + minX;
			y = minY;
			break;
		case 3://bottom
			x = rand() % maxX + minX;
			y = maxY;
			break;
		default:
			//error
			break;
		}

		//type determination
		srand((int)time(0) * i * 2);
		int type = rand() % 3;

		zombies[i].spawn(x, y, type, i);
	}

	return zombies;
}