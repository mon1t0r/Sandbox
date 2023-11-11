#pragma once
#include "material.h"

class Air : public Material
{
	using Material::Material;

public:
	bool IsColorConstant() override;
};

