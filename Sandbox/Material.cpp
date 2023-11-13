#include <stdlib.h>
#include <algorithm>
#include "material.h"
#include "air.h"
#include "sand.h"
#include "water.h"

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

void Material::OnCellUpdate(Field* field, int x, int y)
{
	
}

void Material::OnFieldUpdatePre(Field* field)
{

}

void Material::OnFieldUpdatePost(Field* field)
{

}

void Material::InitMaterials()
{
	if (materials_initialized)
		return;
	materials_initialized = true;

	materials[MaterialType::AIR] = std::make_shared<Air>(Air(0x000000));
	materials[MaterialType::STONE] = std::make_shared<Material>(Material(0x808080));
	materials[MaterialType::SAND] = std::make_shared<Sand>(Sand(0xc2b280));
	materials[MaterialType::WATER] =  std::make_shared<Water>(Water(0x0f5e9c));
}

std::shared_ptr<Material> Material::FromType(MaterialType type)
{
	if (!materials_initialized)
		return nullptr;

	if (type < 0 || type >= MaterialType::MATERIALS_COUNT) {
		return Material::materials[MaterialType::AIR];
	}

	return Material::materials[type];
}