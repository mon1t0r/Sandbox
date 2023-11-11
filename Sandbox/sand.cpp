#include "sand.h"
#include "game.h"

bool Sand::IsCrumblySpawn()
{
    return true;
}

void Sand::OnUpdate(int x, int y)
{
    if (IsAir(x, y - 1))
    {
        MoveCell(x, y, x, y - 1);
        return;
    }

    bool is_air_right = IsAir(x + 1, y - 1);
    bool is_air_left = IsAir(x - 1, y - 1);

    if (is_air_right && is_air_left)
    {
        if (rand() % 2)
            MoveCell(x, y, x + 1, y - 1);
        else
            MoveCell(x, y, x - 1, y - 1);
    }
    else if (is_air_right)
    {
        MoveCell(x, y, x + 1, y - 1);
    }
    else if (is_air_left)
    {
        MoveCell(x, y, x - 1, y - 1);
    }
}