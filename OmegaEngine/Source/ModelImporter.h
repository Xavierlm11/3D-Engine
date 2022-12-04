#include "ModelData.h"
#include "Assimp/include/mesh.h"

namespace ModelImporter
{
	void Import(const aiMesh* mesh, MeshData* ourMesh);
	char* Save(const ModelData* ourMesh, uint& size);// char*& fileBuffer);
	void Load(char* buffer, ModelData* model);
};
