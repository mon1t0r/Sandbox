#include "water.h"
#include "game.h"
#include "cell.h"

bool Water::IsCrumblySpawn()
{
    return true;
}

void Water::OnUpdate(Field* field, int x, int y)
{
    if (field->IsMaterial(x, y - 1, MaterialType::AIR))
    {
        field->MovePoint(x, y, x, y - 1);
        return;
    }

    if (field->IsMaterial(x, y + 1, MaterialType::SAND))
    {
        field->SwapPoints(x, y, x, y + 1);
        return;
    }

    bool is_air_right = field->IsMaterial(x + 1, y, MaterialType::AIR);
    bool is_air_left = field->IsMaterial(x - 1, y, MaterialType::AIR);

    auto cell = field->GetCell(x, y);

    if (is_air_right && is_air_left)
    {
        if (cell->GetInfo())
            field->MovePoint(x, y, x + 1, y);
        else
            field->MovePoint(x, y, x - 1, y);
    }
    else if (is_air_right)
    {
        cell->SetInfo(true);
        field->MovePoint(x, y, x + 1, y);
    }
    else if (is_air_left)
    {
        cell->SetInfo(false);
        field->MovePoint(x, y, x - 1, y);
    }
}