#pragma once
#include "material.h"

class Sand : public Material
{
	using Material::Material;

public:
	bool IsCrumblySpawn() override;
	void OnUpdate(Field* field, int x, int y) override;
};
