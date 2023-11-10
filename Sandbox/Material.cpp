#include <stdlib.h>
#include <algorithm>
#include "Material.h"

#define COLOR_RANDOM_OFFSET 20

Material Material::materials[MATERIALS_COUNT];
bool Material::materials_initialized;

Material::Material() : Material(0x000000)
{
	
}

Material::Material(int color) : Material(color, false)
{
	
}

Material::Material(int color, bool is_color_constant)
{
	this->color = color;
	this->is_color_constant = is_color_constant;
}

int Material::GetBaseColor()
{
	return color;
}

int Material::GetRandomColor()
{
	if (is_color_constant)
		return color;

	int offset = rand() % COLOR_RANDOM_OFFSET;

	int r = (color & 0xFF0000) >> 16;
	int g = (color & 0x00FF00) >> 8;
	int b = (color & 0x0000FF);

	r = std::clamp(r + offset, 0, 255);
	g = std::clamp(g + offset, 0, 255);
	b = std::clamp(b + offset, 0, 255);

	return (r << 16) | (g << 8) | b;
}

void Material::InitMaterials()
{
	if (materials_initialized)
		return;
	materials_initialized = true;

	materials[Material::AIR] = Material(0x000000, true);
	materials[Material::STONE] = Material(0x808080);
}

Material* Material::FromType(Material::Type type)
{
	if (!materials_initialized)
		return nullptr;

	if (type >= Material::MATERIALS_COUNT) {
		return Material::materials + Material::AIR;
	}

	return Material::materials + type;
}