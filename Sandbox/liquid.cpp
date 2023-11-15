#include "liquid.h"
#include "field.h"

MaterialType Liquid::GetType()
{
	return MaterialType::LIQUID;
}

bool Liquid::IsCrumblySpawn()
{
	return true;
}

bool Liquid::CheckCells(Field* field, int x1, int y1, int x2, int y2, bool move)
{
	if (field->IsMaterial(x2, y2, Materials::AIR))
	{
		if (move)
			field->MovePoint(x1, y1, x2, y2);
		return true;
	}

	return false;
}

void Liquid::OnCellUpdate(Field* field, int x, int y)
{
	if (CheckCells(field, x, y, x, y - 1, true))
		return;

	if (rand() % 2)
	{
		if (CheckCells(field, x, y, x - 1, y, false) && CheckCells(field, x, y, x - 1, y - 1, true))
			return;

		if (CheckCells(field, x, y, x + 1, y, false) && CheckCells(field, x, y, x + 1, y - 1, true))
			return;

		if(!CheckCells(field, x, y, x - 1, y, true))
			CheckCells(field, x, y, x + 1, y, true);
	}
	else
	{
		if (CheckCells(field, x, y, x + 1, y, false) && CheckCells(field, x, y, x + 1, y - 1, true))
			return;

		if (CheckCells(field, x, y, x - 1, y, false) && CheckCells(field, x, y, x - 1, y - 1, true))
			return;

		if (!CheckCells(field, x, y, x + 1, y, true))
			CheckCells(field, x, y, x - 1, y, true);
	}
}