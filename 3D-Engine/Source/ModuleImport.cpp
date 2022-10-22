#include "Globals.h"
#include "Application.h"
#include "ModuleImport.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImport::~ModuleImport()
{

}



bool ModuleImport::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

update_status ModuleImport::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleImport::LoadFile(const char* file_path) {
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes()==true)
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else {
		LOG("Error loading scene % s", file_path);
	}
		
}