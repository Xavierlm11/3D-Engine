#include "SDL/include/SDL_stdinc.h"
#include "CMaterial.h"

#include "Assimp/include/scene.h"
#include "MaterialData.h"


namespace MaterialImporter
{
	void LoadTextureLump(const char* buffer, uint size);
	Uint64  Save(char** fileBuffer);
	void Load(const char* fileBuffer, MaterialData* ourMaterial, uint size);
};
