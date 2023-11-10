#pragma once

#include "cell.h"

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 40

extern Cell matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitGame();
void FreeGame();