#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <ctime>
#include <glad/glad.h>
#include <algorithm>
#include <tuple>
#include "game.h"
#include "visual.h"
#include "material.h"

#define DRAW_RADIUS 20

Cell matrix[FIELD_WIDTH][FIELD_HEIGHT];

bool mouse_down;
int mouse_prev_x = -1, mouse_prev_y = -1;

void ConnectDots(int, int, int, int, Material::Type);
bool IsOutOfBounds(int, int);
std::tuple<int, int> GetMousePos(uint32_t, uint32_t);

void InitGame()
{
	Material::InitMaterials();
	srand(time(NULL));
	for (int i = 0; i < FIELD_WIDTH; ++i)
		for (int j = 0; j < FIELD_HEIGHT; ++j)
			matrix[i][j].UpdateMaterial(Material::AIR);
	SetMatrixUpdated();
}

void FreeGame()
{
	
}

void HandleMouseDown(uint32_t x, uint32_t y)
{
	mouse_down = true;

	auto mouse_pos = GetMousePos(x, y);
    mouse_prev_x = std::get<0>(mouse_pos);
    mouse_prev_y = std::get<1>(mouse_pos);
}

void HandleMouseUp(uint32_t x, uint32_t y)
{
	mouse_down = false;
}

void HandleMouseMove(uint32_t x, uint32_t y)
{
	if (!mouse_down)
		return;

	auto mouse_pos = GetMousePos(x, y);
	int xi = std::get<0>(mouse_pos);
	int yi = std::get<1>(mouse_pos);

	ConnectDots(mouse_prev_x, mouse_prev_y, xi, yi, Material::STONE);
    SetMatrixUpdated();

	mouse_prev_x = xi;
	mouse_prev_y = yi;
}

void DrawDot(int x, int y, Material::Type type)
{	
    int x_min = x - DRAW_RADIUS;
    int x_max = x + DRAW_RADIUS;
    int y_min = y - DRAW_RADIUS;
    int y_max = y + DRAW_RADIUS;

    if (IsOutOfBounds(x_min, y_min) && IsOutOfBounds(x_max, y_max))
        return;

    for (int i = x_min; i <= x_max; ++i)
        for (int j = y_min; j <= y_max; ++j)
            if (!IsOutOfBounds(i, j))
                matrix[i][j].UpdateMaterial(type);
}

bool IsOutOfBounds(int x, int y)
{
    return x < 0 || y < 0 || x >= FIELD_WIDTH || y >= FIELD_HEIGHT;
}

std::tuple<int, int> GetMousePos(uint32_t x, uint32_t y)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float xf = ((int)x - viewport[0]) / (float)viewport[2];
    float yf = 1.0f - ((int)y - viewport[1]) / (float)viewport[3];

    return std::make_tuple((int)(xf * FIELD_WIDTH), (int)(yf * FIELD_HEIGHT));
}

//https://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
void ConnectDots(int x1, int y1, int x2, int y2, Material::Type type)
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    DrawDot(x1, y1, type);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            // reduce error, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;

            DrawDot(x1, y1, type);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            // reduce error, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;

            DrawDot(x1, y1, type);
        }
    }
}