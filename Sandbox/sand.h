#pragma once
#include "material.h"

class Sand : public Material
{
	using Material::Material;

public:
	bool IsCrumblySpawn() override;
	void OnUpdate(int x, int y) override;
};

