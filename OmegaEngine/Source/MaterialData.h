#pragma once
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "Glew/include/glew.h"
#include "Resource.h"

class MaterialData : public Resource {

public:
	MaterialData(const char* path);
	~MaterialData() {};

	GLuint texture_id;
};