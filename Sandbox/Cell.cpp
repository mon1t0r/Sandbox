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
	if (!new_material || material == new_material)
		return;
	 
	material = new_material;
	color = material->GetRandomColor();
}

int Cell::GetColor()
{
	return color;
}