#pragma once

class Material;
enum Materials : unsigned int;

class Cell
{
	Materials material = (Materials)0;
	int color = 0;
	bool is_updated = false;
	bool info = false;

public:
	Cell();
	Cell(Materials type);

	void UpdateMaterial(Materials type);
	Materials GetMaterial();

	void CopyFrom(Cell* cell);

	int GetColor();
	void SetUpdated(bool is_updated);
	bool IsUpdated();
	void SetInfo(bool info);
	bool GetInfo();
};