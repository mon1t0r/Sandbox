#pragma once

class Material;
enum MaterialType : unsigned int;

class Cell
{
	MaterialType material = (MaterialType)0;
	int color = 0;
	bool is_updated = false;
	bool info = false;

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