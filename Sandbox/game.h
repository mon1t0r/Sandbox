#pragma once

#include "cell.h"

#define FIELD_WIDTH 800
#define FIELD_HEIGHT 600

extern Cell matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitGame();
void FreeGame();

void HandleMouseDown(uint32_t, uint32_t);
void HandleMouseUp(uint32_t, uint32_t);
void HandleMouseMove(uint32_t, uint32_t);