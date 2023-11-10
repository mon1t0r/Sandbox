#include "Material.h"

Material::Material()
{
	
}

Material::Material(int color)
{
	this->color = color;
}

int Material::GetColor()
{
	return this->color;
}