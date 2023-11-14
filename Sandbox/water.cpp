#include "water.h"
#include "game.h"
#include "cell.h"

Water::~Water()
{
#ifdef COMPLEX_WATER
    if (buffer)
        delete[] buffer;
#endif
}

bool Water::IsCrumblySpawn()
{
    return true;
}

void Water::OnCellUpdate(Field* field, int x, int y)
{
    if (field->IsMaterial(x, y - 1, MaterialType::AIR))
    {
        field->MovePoint(x, y, x, y - 1);
        return;
    }

    /*if (field->IsMaterial(x, y + 1, MaterialType::SAND))
    {
        field->SwapPoints(x, y, x, y + 1);
        return;
    }*/

#ifdef COMPLEX_WATER
    if (field->IsMaterial(x, y - 1, MaterialType::WATER) && !field->IsMaterial(x, y + 1, MaterialType::WATER))
    {
        water_level = y - 1;
        found_y = -1;
        FindPath(field, x, y - 1);
        if (found_y >= 0)
            field->MovePoint(x, y, found_x, found_y);
        else
            OnFieldUpdatePost(field);
    }
#else

    if (field->IsMaterial(x - 1, y - 1, MaterialType::AIR))
    {
        field->MovePoint(x, y, x - 1, y);
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
#endif
}

void Water::OnFieldUpdatePre(Field* field)
{
#ifdef COMPLEX_WATER
    if (!buffer)
    {
        buffer_width = field->GetWidth();
        buffer_height = field->GetHeight();
        buffer = new bool[buffer_width * buffer_height];
    }
#endif
}

void Water::OnFieldUpdatePost(Field* field)
{
#ifdef COMPLEX_WATER
    ResetBuffer();
#endif
}

#ifdef COMPLEX_WATER
void Water::FindPath(Field* field, int x, int y)
{
    if (field->IsOutOfBounds(x, y))
        return;

    if (y <= water_level && (y < found_y || found_y < 0))
    {
        if (field->IsMaterial(x, y, MaterialType::AIR))
        {
            found_x = x;
            found_y = y;
            return;
        }
    }

    Cell* cell;
    if (field->IsMaterial(x, y, MaterialType::WATER) && !buffer[y * buffer_width + x])
    {
        buffer[y * buffer_width + x] = true;
        FindPath(field, x, y - 1);
        FindPath(field, x, y + 1);
        FindPath(field, x - 1, y);
        FindPath(field, x + 1, y);
    }
}

void Water::ResetBuffer()
{
    if (buffer)
        memset(buffer, false, buffer_width * buffer_height * sizeof(bool));
}
#endif