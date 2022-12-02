#include "MeshData.h"
#include "Assimp/include/mesh.h"

namespace MeshImporter
{
	void Import(const aiMesh* mesh, MeshData* ourMesh);
	char* Save(const MeshData* ourMesh, uint& size);// char** fileBuffer);
	void Load(const char* fileBuffer, MeshData* ourMesh);
};
