#include "field.h"
#include "cell.h"
#include "material.h"

Field::Field(int width, int height, void(*matrix_updated_callback) ())
{
	this->width = width;
	this->height = height;
    this->matrix_updated_callback = matrix_updated_callback;

	matrix = new Cell*[width];
    for (int i = 0; i < width; ++i)
    {
        matrix[i] = new Cell[height];
        matrix[i]->UpdateMaterial(MaterialType::AIR);
    }
}

Field::~Field()
{
	for (int i = 0; i < width; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

void Field::Update()
{
    for (int i = 0; i < MaterialType::MATERIALS_COUNT; ++i)
        Material::FromType((MaterialType)i)->OnFieldUpdatePre(this);

    Cell* cell;
	for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            cell = &matrix[i][j];
            if (cell->IsUpdated())
                continue;
            cell->SetUpdated(true);
            Material::FromType(cell->GetMaterial())->OnCellUpdate(this, i, j);
        }

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            matrix[i][j].SetUpdated(false);

    for (int i = 0; i < MaterialType::MATERIALS_COUNT; ++i)
        Material::FromType((MaterialType)i)->OnFieldUpdatePost(this);
}

int Field::GetWidth()
{
    return width;
}

int Field::GetHeight()
{
    return height;
}

void Field::SetPoint(int x, int y, MaterialType type)
{
    if (IsOutOfBounds(x, y))
        return;
    matrix[x][y].UpdateMaterial(type);
    SetMatrixUpdated();
}

void Field::SetPoint(int x, int y, MaterialType type, int radius)
{
    int x_min = x - radius;
    int x_max = x + radius;
    int y_min = y - radius;
    int y_max = y + radius;

    if (IsOutOfBounds(x_min, y_min) && IsOutOfBounds(x_max, y_max)
        && IsOutOfBounds(x_max, y_min) && IsOutOfBounds(x_min, y_max))
        return;

    bool flag = false;
    for (int i = x_min; i <= x_max; ++i)
        for (int j = y_min; j <= y_max; ++j)
        {
            if (!IsOutOfBounds(i, j) && (i - x) * (i - x) + (j - y) * (j - y) < radius * radius &&
                !(radius > 2 && Material::FromType(type)->IsCrumblySpawn() && rand() % 20))
            {
                matrix[i][j].UpdateMaterial(type);
                flag = true;
            }
        } 

    if(flag)
        SetMatrixUpdated();
}

//https://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
void Field::SetLine(int x1, int y1, int x2, int y2, MaterialType type, int line_width)
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    SetPoint(x1, y1, type, line_width);

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

            SetPoint(x1, y1, type, line_width);
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

            SetPoint(x1, y1, type, line_width);
        }
    }
}

void Field::MovePoint(int start_x, int start_y, int end_x, int end_y)
{
    if (IsOutOfBounds(start_x, start_y) || IsOutOfBounds(end_x, end_y) ||
        IsMaterial(start_x, start_y, MaterialType::AIR) || !IsMaterial(end_x, end_y, MaterialType::AIR))
        return;

    matrix[end_x][end_y].CopyFrom(&matrix[start_x][start_y]);
    matrix[start_x][start_y].UpdateMaterial(MaterialType::AIR);

    SetMatrixUpdated();
}

void Field::SwapPoints(int x1, int y1, int x2, int y2)
{
    if (IsOutOfBounds(x1, y1) || IsOutOfBounds(x2, y2))
        return;

    Cell cell = matrix[x1][y1];
    matrix[x1][y1].CopyFrom(&matrix[x2][y2]);
    matrix[x2][y2].CopyFrom(&cell);

    SetMatrixUpdated();
}

Cell* Field::GetCell(int x, int y)
{
    if (IsOutOfBounds(x, y))
        return nullptr;

    return &matrix[x][y];
}

bool Field::IsOutOfBounds(int x, int y)
{
    return x < 0 || y < 0 || x >= width || y >= height;
}

bool Field::IsMaterial(int x, int y, MaterialType type)
{
    if (IsOutOfBounds(x, y))
        return false;

    return matrix[x][y].GetMaterial() == type;
}

inline void Field::SetMatrixUpdated()
{
    matrix_updated_callback();
}