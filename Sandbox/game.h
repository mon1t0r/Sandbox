#pragma once

#include "material.h"

#define FIELD_WIDTH 800
#define FIELD_HEIGHT 400

extern uint8_t matrix[FIELD_WIDTH][FIELD_HEIGHT];
extern Material materials[Material::MATERIALS_COUNT];

void InitGame();
void FreeGame();