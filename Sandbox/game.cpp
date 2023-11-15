#include <cstdlib>
#include <iostream>
#include <ctime>
#include <glad/glad.h>
#include <algorithm>
#include <tuple>
#include "game.h"
#include "visual.h"
#include "material.h"

#define DRAW_RADIUS 3

void HandleMouse();
std::tuple<int, int> GetMousePos(uint32_t, uint32_t);

Field* game_field;

HWND hwnd;

bool mouse_down;
int mouse_prev_x = -1, mouse_prev_y = -1;
Materials current_material = Materials::STONE;

void InitGame(HWND handle)
{
    hwnd = handle;

    srand(time(NULL));

	Material::InitMaterials();
    game_field = new Field(FIELD_WIDTH, FIELD_HEIGHT, &SetMatrixUpdated);
}

void FreeGame()
{
    delete game_field;
}

void UpdateGame()
{
    game_field->Update();

    HandleMouse();
}

void HandleMouse()
{
    if ((GetKeyState(VK_LBUTTON) & 0x8000) == 0)
        return;

    POINT p;
    RECT rect;
    GetCursorPos(&p);

    if (!ScreenToClient(hwnd, &p) || !GetClientRect(hwnd, &rect))
        return;

    if (rect.left > p.x || rect.right < p.x || rect.top > p.y || rect.bottom < p.y)
        return;

	auto mouse_pos = GetMousePos(p.x, p.y);
	int xi = std::get<0>(mouse_pos);
	int yi = std::get<1>(mouse_pos);

    game_field->SetLine(mouse_prev_x, mouse_prev_y, xi, yi, current_material, DRAW_RADIUS);

	mouse_prev_x = xi;
	mouse_prev_y = yi;
}

void HandleMouseDown(uint32_t x, uint32_t y)
{
    auto mouse_pos = GetMousePos(x, y);
    mouse_prev_x = std::get<0>(mouse_pos);
    mouse_prev_y = std::get<1>(mouse_pos);

    game_field->SetPoint(mouse_prev_x, mouse_prev_y, current_material, DRAW_RADIUS);
}

void SwitchMaterial()
{
    int ind = current_material + 1;
    if (ind >= Materials::MATERIALS_COUNT)
        ind = 0;
    current_material = (Materials) ind;
}

std::tuple<int, int> GetMousePos(uint32_t x, uint32_t y)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float xf = ((int)x - viewport[0]) / (float)viewport[2];
    float yf = 1.0f - ((int)y - viewport[1]) / (float)viewport[3];

    return std::make_tuple((int)(xf * FIELD_WIDTH), (int)(yf * FIELD_HEIGHT));
}