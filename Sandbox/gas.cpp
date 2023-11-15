#include "gas.h"
#include "field.h"

MaterialType Gas::GetType()
{
	return MaterialType::GAS;
}

bool Gas::IsCrumblySpawn()
{
	return true;
}

void Gas::OnCellUpdate(Field* field, int x, int y)
{

}