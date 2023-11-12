#pragma once
#include "material.h"

class Water : public Material
{
	using Material::Material;

public:
	bool IsCrumblySpawn() override;
	void OnUpdate(Field* field, int x, int y) override;
};

