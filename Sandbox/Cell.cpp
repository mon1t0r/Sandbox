#include "cell.h"
#include "material.h"

Cell::Cell() : Cell(MaterialType::AIR)
{
	
}

Cell::Cell(MaterialType type)
{
	UpdateMaterial(type);
}

void Cell::UpdateMaterial(MaterialType type)
{
	auto new_material = Material::FromType(type);
	if (!new_material || material == type)
		return;
	 
	material = type;
	color = new_material->GetRandomColor();
}

MaterialType Cell::GetMaterial()
{
	return material;
}

void Cell::CopyFrom(Cell* cell)
{
	material = cell->material;
	color = cell->color;
}

int Cell::GetColor()
{
	return color;
}