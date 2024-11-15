#pragma once
#include "material.h"

class Gas : public Material
{
	using Material::Material;

public:
	MaterialType GetType() override;
	bool IsCrumblySpawn() override;
	void OnCellUpdate(Field* field, int x, int y) override;
};

