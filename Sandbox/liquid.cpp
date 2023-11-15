#include "liquid.h"
#include "field.h"

MaterialType Liquid::GetType()
{
	return MaterialType::LIQUID;
}

bool Liquid::IsCrumblySpawn()
{
	return true;
}

void Liquid::OnCellUpdate(Field* field, int x, int y)
{

}