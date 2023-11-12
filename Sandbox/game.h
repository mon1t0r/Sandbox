#pragma once

#include <windows.h>
#include <cstdint>
#include "field.h"

#define FIELD_WIDTH 50
#define FIELD_HEIGHT 25

void InitGame(HWND);
void FreeGame();
void UpdateGame();

extern Field* game_field;

void HandleMouseDown(uint32_t, uint32_t);
void SwitchMaterial();