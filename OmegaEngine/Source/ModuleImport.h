#pragma once
#include "Module.h"
#include "Globals.h"
#include "MeshData.h"
#include "ModelData.h"
#include "MaterialData.h"
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
	void GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes);
	MeshData* GetMeshData(aiMesh* mesh, const aiScene * scene);
	void ReleaseFile(const aiScene* scene);

	//Textures
	void LoadCheckerTexture();

	GLuint ImportTexture(const char* path);

};
