#include <stdlib.h>
#include <stdint.h>
#include <ctime>
#include "game.h"
#include "visual.h"
#include "material.h"

Cell matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitGame()
{
	Material::InitMaterials();
	srand(time(NULL));
	for (int i = 0; i < FIELD_WIDTH; ++i)
		for (int j = 0; j < FIELD_HEIGHT; ++j)
			matrix[i][j] = (rand() % 2) ? Cell(Material::AIR) : Cell(Material::STONE);
	SetMatrixUpdated();
}

void FreeGame()
{
	
}