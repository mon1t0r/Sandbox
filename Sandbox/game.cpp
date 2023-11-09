#include "game.h"

bool matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitializeGame()
{
	for (int i = 0; i < FIELD_WIDTH; ++i)
		for (int j = 0; j < FIELD_HEIGHT; ++j)
			matrix[i][j] = 0;
}