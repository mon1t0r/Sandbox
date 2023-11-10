#pragma once

class Material
{
	int color;
	bool is_color_constant;
	
public:
	Material();
	Material(int color);
	Material(int color, bool is_color_constant);
	int GetBaseColor();
	int GetRandomColor();

	// Used for array indexes
	enum Type
	{
		AIR,
		STONE,
		MATERIALS_COUNT
	};

	static void InitMaterials();
	static Material* FromType(Type type);

private:
	static Material materials[MATERIALS_COUNT];
	static bool materials_initialized;
};