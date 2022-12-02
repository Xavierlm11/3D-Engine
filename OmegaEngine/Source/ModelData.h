#pragma once
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "Glew/include/glew.h"
#include "Resource.h"
#include "MeshData.h"
class ModelData : public Resource{

public:
	ModelData(const char* path);
	ModelData();
	~ModelData() {};

	std::vector<MeshData*> meshDatas;
	std::vector<MaterialData*> materialDatas;
};