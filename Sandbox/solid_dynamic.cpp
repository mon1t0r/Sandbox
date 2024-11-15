#include "solid_dynamic.h"
#include "field.h"

MaterialType SolidDynamic::GetType()
{
	return MaterialType::SOLID;
}

bool SolidDynamic::IsCrumblySpawn()
{
	return true;
}

bool SolidDynamic::CheckCells(Field* field, int x1, int y1, int x2, int y2, bool move)
{
	if (field->IsMaterial(x2, y2, Materials::AIR))
	{
		if(move)
			field->MovePoint(x1, y1, x2, y2);
		return true;
	}

	if (field->IsMaterialType(x2, y2, MaterialType::LIQUID))
	{
		if (move)
			field->SwapPoints(x1, y1, x2, y2);
		return true;
	}

	return false;
}

void SolidDynamic::OnCellUpdate(Field* field, int x, int y)
{
	if (CheckCells(field, x, y, x, y - 1, true))
		return;

	if (CheckCells(field, x, y, x - 1, y, false) && CheckCells(field, x, y, x - 1, y - 1, true))
		return;

	if (CheckCells(field, x, y, x + 1, y, false) && CheckCells(field, x, y, x + 1, y - 1, true))
		return;
}