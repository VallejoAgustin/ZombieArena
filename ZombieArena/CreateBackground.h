#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
using namespace sf;

int createBackground(VertexArray &, IntRect);
bool isEdge(int, int, int, int);