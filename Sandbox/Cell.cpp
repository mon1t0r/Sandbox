#include "Cell.h"

Cell::Cell() : Cell(Material::AIR)
{
	
}

Cell::Cell(Material::Type type)
{
	this->material = Material::FromType(type);
	color = material->GetRandomColor();
}

int Cell::GetColor()
{
	return color;
}