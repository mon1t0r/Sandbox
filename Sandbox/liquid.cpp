#include "liquid.h"
#include "field.h"
#include "cell.h"

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

bool Liquid::TryMoveTo(Field* field, int x, int y, bool right)
{
	int i;
	if (right)
	{
		for (i = x + 1; i < x + dispersion_rate; ++i)
		{
			if (!(rand() % dispersion_rate) || !field->IsMaterial(i, y, Materials::AIR))
			{
				i--;
				break;
			}
		}
	}
	else
	{
		for (i = x - 1; i > x - dispersion_rate; --i)
		{
			if (!(rand() % dispersion_rate) || !field->IsMaterial(i, y, Materials::AIR))
			{
				i++;
				break;
			}
		}
	}

	if (!field->IsMaterial(i, y, Materials::AIR))
		return false;

	field->MovePoint(x, y, i, y);
	field->GetCell(i, y)->SetInfo(i > x);

	return true;
}

void Liquid::OnCellUpdate(Field* field, int x, int y)
{
	if (CheckCells(field, x, y, x, y - 1, true))
		return;

	if (field->GetCell(x, y)->GetInfo())
	{
		if (CheckCells(field, x, y, x + 1, y, false) && CheckCells(field, x, y, x + 1, y - 1, true))
		{
			field->GetCell(x + 1, y)->SetInfo(true);
			return;
		}

		if (CheckCells(field, x, y, x - 1, y, false) && CheckCells(field, x, y, x - 1, y - 1, true))
		{
			field->GetCell(x - 1, y)->SetInfo(false);
			return;
		}

		if (!TryMoveTo(field, x, y, true))
			TryMoveTo(field, x, y, false);
	}
	else
	{
		if (CheckCells(field, x, y, x - 1, y, false) && CheckCells(field, x, y, x - 1, y - 1, true))
		{
			field->GetCell(x - 1, y)->SetInfo(false);
			return;
		}

		if (CheckCells(field, x, y, x + 1, y, false) && CheckCells(field, x, y, x + 1, y - 1, true))
		{
			field->GetCell(x + 1, y)->SetInfo(true);
			return;
		}

		if (!TryMoveTo(field, x, y, false))
			TryMoveTo(field, x, y, true);
	}
}