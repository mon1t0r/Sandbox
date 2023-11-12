#pragma once

class Material;
enum MaterialType : unsigned int;

class Cell
{
	MaterialType material;
	int color;
	bool is_updated;
	bool info;

public:
	Cell();
	Cell(MaterialType type);

	void UpdateMaterial(MaterialType type);
	MaterialType GetMaterial();

	void CopyFrom(Cell* cell);

	int GetColor();
	void SetUpdated(bool is_updated);
	bool IsUpdated();
	void SetInfo(bool info);
	bool GetInfo();
};