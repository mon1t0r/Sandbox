#include "Cell.h"

Cell::Cell() : Cell(Material::AIR)
{
	
}

Cell::Cell(Material::Type type)
{
	UpdateMaterial(type);
}

void Cell::UpdateMaterial(Material::Type type)
{
	auto new_material = Material::FromType(type);
	if (!new_material || material == type)
		return;
	 
	material = type;
	color = new_material->GetRandomColor();
}

void Cell::CopyFrom(Cell* cell)
{
	material = cell->material;
	color = cell->color;
}

Material::Type Cell::GetMaterial()
{
	return material;
}

int Cell::GetColor()
{
	return color;
}