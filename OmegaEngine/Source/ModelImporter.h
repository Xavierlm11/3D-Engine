#include "ModelData.h"
#include "Assimp/include/mesh.h"

namespace ModelImporter
{
	void Import(const aiMesh* mesh, MeshData* ourMesh);
	Uint64  Save(const MeshData* ourMesh, char** fileBuffer);
	void Load(const char* fileBuffer, MeshData* ourMesh);
};
