#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"

class MeshData {

public:
	MeshData();
	~MeshData();

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
};
