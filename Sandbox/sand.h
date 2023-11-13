#pragma once
#include "material.h"

class Sand : public Material
{
	using Material::Material;

public:
	bool IsCrumblySpawn() override;
	void OnCellUpdate(Field* field, int x, int y) override;
};

