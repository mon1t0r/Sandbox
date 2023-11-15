#pragma once
#include "material.h"

class SolidStatic : public Material
{
	using Material::Material;

public:
	MaterialType GetType() override;
};

