#pragma once

#include <memory>

// Used for array indexes
enum MaterialType : unsigned int
{
	AIR,
	STONE,
	SAND,
	WATER,
	MATERIALS_COUNT
};

class Field;

class Material
{
	int color;
	
public:
	Material();
	Material(int color);

	int GetBaseColor();
	int GetRandomColor();

	virtual bool IsColorConstant();
	virtual bool IsCrumblySpawn();

	virtual void OnUpdate(Field* field, int x, int y);

	static void InitMaterials();
	static std::shared_ptr<Material> FromType(MaterialType type);

private:
	static std::shared_ptr<Material> materials[MaterialType::MATERIALS_COUNT];
	static bool materials_initialized;
};