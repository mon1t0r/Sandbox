#pragma once
#include "material.h"

class Liquid : public Material
{
	using Material::Material;

public:
	MaterialType GetType() override;
	bool IsCrumblySpawn() override;
	void OnCellUpdate(Field* field, int x, int y) override;

protected:
	int dispersion_rate = 5;

private:
	bool CheckCells(Field* field, int x1, int y1, int x2, int y2, bool move);
	bool TryMoveTo(Field* field, int x, int y, bool right);
};

