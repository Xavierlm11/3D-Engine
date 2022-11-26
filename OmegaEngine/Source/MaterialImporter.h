#include "SDL/include/SDL_stdinc.h"
#include "CMaterial.h"

#include "Assimp/include/scene.h"

namespace MaterialImporter
{
	void Import(const aiMaterial* material, CMaterial* ourMaterial);
	Uint64  Save(const CMaterial* ourMaterial, char** fileBuffer);
	void Load(const char* fileBuffer, CMaterial* ourMaterial);
};
