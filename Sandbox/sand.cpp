#include "sand.h"
#include "game.h"

bool Sand::IsCrumblySpawn()
{
    return true;
}

bool CanMove(Field* field, int x, int y)
{
    return (field->IsMaterial(x, y - 1, Materials::AIR) &&
        field->IsMaterial(x, y, Materials::AIR)) ||
        (field->IsMaterial(x, y - 1, Materials::WATER) &&
            field->IsMaterial(x, y, Materials::WATER));
}

void Sand::OnCellUpdate(Field* field, int x, int y)
{
    if (field->IsMaterial(x, y - 1, Materials::AIR))
    {
        field->MovePoint(x, y, x, y - 1);
        return;
    }

    if (field->IsMaterial(x, y - 1, Materials::WATER))
    {
        field->SwapPoints(x, y, x, y - 1);
        Material::FromType(Materials::WATER)->OnCellUpdate(field, x, y);
        return;
    }

    bool is_air_right = CanMove(field, x + 1, y);
    bool is_air_left = CanMove(field, x - 1, y);

    if (is_air_right && is_air_left)
    {
        if (rand() % 2)
            field->SwapPoints(x, y, x + 1, y - 1);
        else
            field->SwapPoints(x, y, x - 1, y - 1);
    }
    else if (is_air_right)
    {
        field->SwapPoints(x, y, x + 1, y - 1);
    }
    else if (is_air_left)
    {
        field->SwapPoints(x, y, x - 1, y - 1);
    }
}