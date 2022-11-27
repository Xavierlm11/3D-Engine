#include "SDL/include/SDL_stdinc.h"
#include "CMaterial.h"

#include "Assimp/include/scene.h"

namespace MaterialImporter
{
	void Import(const aiMaterial* material, MaterialData* ourMaterial);
	Uint64  Save(const MaterialData* ourMaterial, char** fileBuffer);
	void Load(const char* fileBuffer, MaterialData* ourMaterial);
};
