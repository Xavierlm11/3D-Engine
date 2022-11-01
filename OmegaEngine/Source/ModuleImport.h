#pragma once
#include "Module.h"
#include "Globals.h"
#include "MeshData.h"
#include "ModelData.h"
#include "GameObject.h"
#include <vector>
#include "Component.h"
#include "Assimp/include/scene.h"

class Component;

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
	void GetMeshDatasObj(const aiScene* scene, const char* name);
	MeshData* GetMeshData(MeshData * meshData, aiMesh* mesh, const aiScene * scene);
	MeshData* GetMeshDataObj(MeshData* meshData, aiMesh* mesh, const aiScene* scene, GameObject* obj);
	void ReleaseFile(const aiScene* scene);

	//Textures
	void LoadCheckerTexture();
	
	GLuint ImportTexture(const char* path);

	std::vector<GameObject*>tgo;

	
	//Component::Types type;
	
};
