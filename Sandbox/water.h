#pragma once
#include "material.h"

//#define COMPLEX_WATER

class Water : public Material
{
	using Material::Material;

public:
	~Water();
	bool IsCrumblySpawn() override;
	void OnCellUpdate(Field* field, int x, int y) override;
	void OnFieldUpdatePre(Field* field) override;
	void OnFieldUpdatePost(Field* field) override;

private:
#ifdef COMPLEX_WATER
	int found_x, found_y, water_level;
	int buffer_width, buffer_height;
	bool* buffer = nullptr;

	void FindPath(Field* field, int x, int y);
	void ResetBuffer();
#else
	void MoveCells(Field* field, int x, int y, bool right);
#endif
};

