#pragma once
#include "Material.h"

class Cell
{
	Material* material;
	int color;

public:
	Cell();
	Cell(Material::Type type);
	int GetColor();
};

