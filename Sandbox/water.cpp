#include "water.h"
#include "game.h"

bool Water::IsCrumblySpawn()
{
    return true;
}

void Water::OnUpdate(Field* field, int x, int y)
{
    if (field->IsMaterial(x, y - 1, MaterialType::AIR))
    {
        field->MoveCell(x, y, x, y - 1);
        return;
    }

    bool is_air_right = field->IsMaterial(x + 1, y - 1, MaterialType::AIR) && field->IsMaterial(x + 1, y, MaterialType::AIR);
    bool is_air_left = field->IsMaterial(x - 1, y - 1, MaterialType::AIR) && field->IsMaterial(x - 1, y, MaterialType::AIR);;

    if (is_air_right && is_air_left)
    {
        if (rand() % 2)
            field->MoveCell(x, y, x + 1, y - 1);
        else
            field->MoveCell(x, y, x - 1, y - 1);
    }
    else if (is_air_right)
    {
        field->MoveCell(x, y, x + 1, y - 1);
    }
    else if (is_air_left)
    {
        field->MoveCell(x, y, x - 1, y - 1);
    }
}