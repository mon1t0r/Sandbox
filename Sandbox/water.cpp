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

    auto cell = field->GetCell(x, y);
    MoveCells(field, x, y, cell->GetInfo());

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
#else
void Water::MoveCells(Field* field, int x, int y, bool right)
{
    bool is_air_right = field->IsMaterial(x + 1, y, MaterialType::AIR);
    bool is_air_left = field->IsMaterial(x - 1, y, MaterialType::AIR);

    if (right && !is_air_right)
    {
        field->GetCell(x, y)->SetInfo(false);
        return;
    }
    else if (!right && !is_air_left)
    {
        field->GetCell(x, y)->SetInfo(true);
        return;
    }

    if (!(right && is_air_right) && !(!right && is_air_left))
        return;

    if (right)
    {
        is_air_right = field->IsMaterial(x + 2, y, MaterialType::AIR);

        field->GetCell(x, y)->SetInfo(is_air_right);
        field->MovePoint(x, y, x + 1, y);

        bool flag = true;
        int x_ind = x;
        while (x_ind > 0 && field->IsMaterial(x_ind - 1, y, MaterialType::WATER))
        {
            field->GetCell(x_ind - 1, y)->SetInfo(is_air_right);
            field->MovePoint(x_ind - 1, y, x_ind, y);
            if (field->IsMaterial(x + x - x_ind + 1, y - 1, MaterialType::AIR))
                field->MovePoint(x_ind, y, x + x - x_ind + 1, y - 1);
            x_ind--;
        }
        if (!is_air_right && x_ind != x)
            field->GetCell(x_ind + 1, y)->SetInfo(true);
    }
    else
    {
        is_air_left = field->IsMaterial(x - 2, y, MaterialType::AIR);

        field->GetCell(x, y)->SetInfo(!is_air_left);
        field->MovePoint(x, y, x - 1, y);

        int width = field->GetWidth();
        int x_ind = x;
        while (x_ind < width && field->IsMaterial(x_ind + 1, y, MaterialType::WATER))
        {
            field->GetCell(x_ind + 1, y)->SetInfo(!is_air_left);
            field->MovePoint(x_ind + 1, y, x_ind, y);
            if (field->IsMaterial(x + x - x_ind - 1, y - 1, MaterialType::AIR))
                field->MovePoint(x_ind, y, x + x - x_ind - 1, y - 1);
            x_ind++;
        }
        if (!is_air_left && x_ind != x)
            field->GetCell(x_ind - 1, y)->SetInfo(false);
    }
}
#endif