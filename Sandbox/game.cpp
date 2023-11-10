#include <stdlib.h>
#include <ctime>
#include "game.h"
#include "visual.h"

bool matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitGame()
{
	srand(time(NULL));
	for (int i = 0; i < FIELD_WIDTH; ++i)
		for (int j = 0; j < FIELD_HEIGHT; ++j)
			SetScreenTexturePixel(i, j, matrix[i][j] = rand() % 2);
}