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
	is_updated = cell->is_updated;
	info = cell->info;
}

int Cell::GetColor()
{
	return color;
}

void Cell::SetUpdated(bool is_updated)
{
	this->is_updated = is_updated;
}

bool Cell::IsUpdated()
{
	return is_updated;
}

void Cell::SetInfo(bool info)
{
	this->info = info;
}

bool Cell::GetInfo()
{
	return info;
}