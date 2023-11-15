#pragma once

#include <memory>

// Used for array indexes
enum Materials : unsigned int
{
	AIR,
	STONE,
	SAND,
	WATER,
	MATERIALS_COUNT
};

enum MaterialType : unsigned int
{
	NONE,
	SOLID,
	LIQUID,
	GAS
};

class Field;

class Material
{
	int color = 0;
	
public:
	Material();
	Material(int color);

	int GetBaseColor();
	int GetRandomColor();

	virtual MaterialType GetType();
	virtual bool IsColorConstant();
	virtual bool IsCrumblySpawn();

	virtual void OnCellUpdate(Field* field, int x, int y);
	virtual void OnFieldUpdatePre(Field* field);
	virtual void OnFieldUpdatePost(Field* field);

	static void InitMaterials();
	static std::shared_ptr<Material> FromType(Materials type);

private:
	static std::shared_ptr<Material> materials[Materials::MATERIALS_COUNT];
	static bool materials_initialized;
};