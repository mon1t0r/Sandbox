#pragma once

#include <windows.h>
#include "cell.h"

#define FIELD_WIDTH 500
#define FIELD_HEIGHT 250

extern Cell matrix[FIELD_WIDTH][FIELD_HEIGHT];

void InitGame(HWND);
void FreeGame();
void UpdateGame();

void HandleMouseDown(uint32_t, uint32_t);
void SwitchMaterial();