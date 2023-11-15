#include "field.h"
#include "cell.h"
#include "material.h"

Field::Field(int width, int height, void(*matrix_updated_callback) ())
{
	this->width = width;
	this->height = height;
    this->matrix_updated_callback = matrix_updated_callback;

	matrix_main = new Cell*[width];
    for (int i = 0; i < width; ++i)
    {
        matrix_main[i] = new Cell[height];
        matrix_main[i]->UpdateMaterial(Materials::AIR);
    }

    matrix_second = new Cell* [width];
    for (int i = 0; i < width; ++i)
        matrix_second[i] = new Cell[height];
}

Field::~Field()
{
    for (int i = 0; i < width; ++i)
        delete[] matrix_second[i];
    delete[] matrix_second;

	for (int i = 0; i < width; ++i)
		delete[] matrix_main[i];
	delete[] matrix_main;
}

void Field::Update()
{
    processing_update = true;

    for (int i = 0; i < width; ++i)
        memcpy(matrix_second[i], matrix_main[i], height * sizeof(Cell));

    for (int i = 0; i < Materials::MATERIALS_COUNT; ++i)
        Material::FromType((Materials)i)->OnFieldUpdatePre(this);

    Cell* cell;
	for (int j = 0; j < height; ++j)
        for (int i = 0; i < width; ++i)
        {
            cell = &matrix_main[i][j];
            if (cell->IsUpdated())
                continue;
            cell->SetUpdated(true);
            Material::FromType(cell->GetMaterial())->OnCellUpdate(this, i, j);
        }

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            matrix_main[i][j].SetUpdated(false);

    for (int i = 0; i < Materials::MATERIALS_COUNT; ++i)
        Material::FromType((Materials)i)->OnFieldUpdatePost(this);

    processing_update = false;
}

int Field::GetWidth()
{
    return width;
}

int Field::GetHeight()
{
    return height;
}

void Field::SetPoint(int x, int y, Materials material)
{
    if (IsOutOfBounds(x, y))
        return;
    matrix_main[x][y].UpdateMaterial(material);
    SetMatrixUpdated();
}

void Field::SetPoint(int x, int y, Materials material, int radius)
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
                !(radius > 2 && Material::FromType(material)->IsCrumblySpawn() && rand() % 20))
            {
                matrix_main[i][j].UpdateMaterial(material);
                flag = true;
            }
        } 

    if(flag)
        SetMatrixUpdated();
}

//https://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
void Field::SetLine(int x1, int y1, int x2, int y2, Materials material, int line_width)
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    SetPoint(x1, y1, material, line_width);

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

            SetPoint(x1, y1, material, line_width);
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

            SetPoint(x1, y1, material, line_width);
        }
    }
}

void Field::MovePoint(int start_x, int start_y, int end_x, int end_y)
{
    if (IsOutOfBounds(start_x, start_y) || IsOutOfBounds(end_x, end_y) ||
        IsMaterial(start_x, start_y, Materials::AIR) || !IsMaterial(end_x, end_y, Materials::AIR))
        return;

    matrix_main[end_x][end_y].CopyFrom(&matrix_main[start_x][start_y]);
    matrix_main[start_x][start_y].UpdateMaterial(Materials::AIR);

    SetMatrixUpdated();
}

void Field::SwapPoints(int x1, int y1, int x2, int y2)
{
    if (IsOutOfBounds(x1, y1) || IsOutOfBounds(x2, y2))
        return;

    Cell cell = matrix_main[x1][y1];
    matrix_main[x1][y1].CopyFrom(&matrix_main[x2][y2]);
    matrix_main[x2][y2].CopyFrom(&cell);

    SetMatrixUpdated();
}

Cell* Field::GetCell(int x, int y)
{
    if (IsOutOfBounds(x, y))
        return nullptr;

    return processing_update ? &matrix_second[x][y] : &matrix_main[x][y];
}

bool Field::IsOutOfBounds(int x, int y)
{
    return x < 0 || y < 0 || x >= width || y >= height;
}

bool Field::IsMaterial(int x, int y, Materials material)
{
    if (IsOutOfBounds(x, y))
        return false;

    return (processing_update ? matrix_second[x][y] : matrix_main[x][y]).GetMaterial() == material;
}

bool Field::IsMaterialType(int x, int y, MaterialType type)
{
    if (IsOutOfBounds(x, y))
        return false;

    return Material::FromType((processing_update ? matrix_second[x][y] : matrix_main[x][y]).GetMaterial())->GetType() == type;
}

inline void Field::SetMatrixUpdated()
{
    matrix_updated_callback();
}