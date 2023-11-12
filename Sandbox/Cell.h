#pragma once

class Material;
enum MaterialType : unsigned int;

class Cell
{
	MaterialType material;
	int color;

public:
	Cell();
	Cell(MaterialType type);

	void UpdateMaterial(MaterialType type);
	MaterialType GetMaterial();

	void CopyFrom(Cell* cell);

	int GetColor();
};