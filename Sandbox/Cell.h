#pragma once
#include "Material.h"

class Cell
{
	Material::Type material;
	int color;

public:
	Cell();
	Cell(Material::Type type);
	void UpdateMaterial(Material::Type type);
	void CopyFrom(Cell* cell);
	Material::Type GetMaterial();
	int GetColor();
};

