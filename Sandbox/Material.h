#pragma once

class Material
{
	int color = 0x000000;
	
public:
	Material();
	Material(int color);
	int GetColor();

	// Used for array indexes
	enum Type
	{
		AIR,
		STONE,
		MATERIALS_COUNT
	};
};