#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "Glew/include/glew.h"

class ModelData {

public:
	ModelData() {};
	~ModelData() {};

	std::vector<MeshData*> meshes;
};