#pragma once
#include "material.h"

class Cell
{
	Material::Type material;
	int color;

public:
	Cell();
	Cell(Material::Type type);

	void UpdateMaterial(Material::Type type);
	Material::Type GetMaterial();

	void CopyFrom(Cell* cell);

	int GetColor();
};

