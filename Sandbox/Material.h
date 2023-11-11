#pragma once

#include <memory>

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

	virtual void OnUpdate(int x, int y);

	// Used for array indexes
	enum Type
	{
		AIR,
		STONE,
		SAND,
		WATER,
		MATERIALS_COUNT
	};

	static void InitMaterials();
	static std::shared_ptr<Material> FromType(Type type);

private:
	static std::shared_ptr<Material> materials[MATERIALS_COUNT];
	static bool materials_initialized;
};