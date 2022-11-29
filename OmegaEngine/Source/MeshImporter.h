#include "MeshData.h"
#include "Assimp/include/mesh.h"

namespace MeshImporter
{
	void Import(const aiMesh* mesh, MeshData* ourMesh);
	char* Save(const MeshData* ourMesh, char** fileBuffer);
	void Load(const char* fileBuffer, MeshData* ourMesh);
};
