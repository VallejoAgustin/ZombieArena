#include "stdafx.h"
#include "CreateBackground.h"

int createBackground(VertexArray & vA, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	//find world height/width in number of tiles
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//set prim type and resize accordingly
	vA.setPrimitiveType(Quads);
	vA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int i = 0; i < worldWidth; i++) {
		for (int j = 0; j < worldHeight; j++) {
			//goes through each vertex in quad
			vA[currentVertex + 0].position = Vector2f(i * TILE_SIZE, j * TILE_SIZE);
			vA[currentVertex + 1].position = Vector2f((i * TILE_SIZE) + TILE_SIZE, j * TILE_SIZE);
			vA[currentVertex + 2].position = Vector2f((i * TILE_SIZE) + TILE_SIZE, (j * TILE_SIZE) + TILE_SIZE);
			vA[currentVertex + 3].position = Vector2f(i * TILE_SIZE, (j * TILE_SIZE) + TILE_SIZE);
			
			if (isEdge(i, j, worldHeight, worldWidth)) {
				vA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				vA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				vA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				vA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else {
				srand((int)time(0) + i * j - i);
				int mOrG = rand() % TILE_TYPES;//mud or grass
				int verticalOffset = mOrG * TILE_SIZE;

				vA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				vA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				vA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				vA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
			}
			
			//'skips' to next quad
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}

bool isEdge(int h, int w, int wH, int wW) {
	if (h == 0 || h == wH - 1 || w == 0 || w == wW - 1) {
		return true;
	}
	return false;
}