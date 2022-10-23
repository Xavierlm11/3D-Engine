#pragma once
#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "Assimp/include/scene.h"

class ModuleImport : public Module
{
public:
	ModuleImport(Application* app, bool start_enabled = true);
	~ModuleImport();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();
	

	const aiScene* LoadFile(const char* path);
	MeshData GetMeshData(aiMesh* scene);
	void ReleaseFile(const aiScene* scene);
};
