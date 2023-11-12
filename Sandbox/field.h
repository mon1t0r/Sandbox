#pragma once

class Cell;
enum MaterialType : unsigned int;

class Field
{
	Cell** matrix;
	int width, height;
	void(*matrix_updated_callback) ();

public:
	Field(int width, int height, void(*matrix_updated_callback) ());
	~Field();

	void Update();

	int GetWidth();
	int GetHeight();

	void SetPoint(int x, int y, MaterialType type);
	void SetPoint(int x, int y, MaterialType type, int radius);
	void SetLine(int x1, int y1, int x2, int y2, MaterialType type, int line_width);

	void MovePoint(int start_x, int start_y, int end_x, int end_y);
	void SwapPoints(int x1, int y1, int x2, int y2);

	Cell* GetCell(int x, int y);

	bool IsOutOfBounds(int x, int y);
	bool IsMaterial(int x, int y, MaterialType type);

private:
	inline void SetMatrixUpdated();
};