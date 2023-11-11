#include <stdlib.h>
#include <algorithm>
#include "material.h"
#include "air.h"
#include "sand.h"

#define COLOR_RANDOM_OFFSET 20

std::shared_ptr<Material> Material::materials[MATERIALS_COUNT];
bool Material::materials_initialized;

Material::Material() : Material(0x000000)
{
	
}

Material::Material(int color)
{
	this->color = color;
}

int Material::GetBaseColor()
{
	return color;
}

int Material::GetRandomColor()
{
	if (IsColorConstant())
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

bool Material::IsColorConstant()
{
	return false;
}

bool Material::IsCrumblySpawn()
{
	return false;
}

void Material::OnUpdate(int x, int y)
{
	
}

void Material::InitMaterials()
{
	if (materials_initialized)
		return;
	materials_initialized = true;

	materials[Material::AIR] = std::make_shared<Air>(Air(0x000000));
	materials[Material::STONE] = std::make_shared<Material>(Material(0x808080));
	materials[Material::SAND] = std::make_shared<Sand>(Sand(0xc2b280));
	materials[Material::WATER] = std::make_shared<Material>(Material(0x0f5e9c));
}

std::shared_ptr<Material> Material::FromType(Material::Type type)
{
	if (!materials_initialized)
		return nullptr;

	if (type >= Material::MATERIALS_COUNT) {
		return Material::materials[Material::AIR];
	}

	return Material::materials[type];
}