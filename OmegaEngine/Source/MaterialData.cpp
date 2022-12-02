#include "MaterialData.h"

MaterialData::MaterialData(const char* path) : Resource(path, Resource::Types::MATERIAL)
{
	texture_id = 0;
}
