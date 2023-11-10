#include <stdlib.h>
#include <stdint.h>
#include <ctime>
#include "game.h"
#include "visual.h"

uint8_t matrix[FIELD_WIDTH][FIELD_HEIGHT];
Material materials[Material::MATERIALS_COUNT];

void InitGame()
{
	materials[Material::AIR] = Material(0x000000);
	materials[Material::STONE] = Material(0x808080);

	srand(time(NULL));
	for (int i = 0; i < FIELD_WIDTH; ++i)
		for (int j = 0; j < FIELD_HEIGHT; ++j)
			matrix[i][j] = rand() % Material::MATERIALS_COUNT;
	SetMatrixUpdated();
}

void FreeGame()
{
	
}