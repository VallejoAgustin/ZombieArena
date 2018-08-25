#ifndef CREATEBACKGROUND_H
#define CREATEBACKGROUND_H

#include "stdafx.h"
#include "Zombie.h"
#include <SFML/Graphics.hpp>
using namespace sf;

int createBackground(VertexArray &, IntRect);
bool isEdge(int, int, int, int);
Zombie* createHorde(int, IntRect);

#endif