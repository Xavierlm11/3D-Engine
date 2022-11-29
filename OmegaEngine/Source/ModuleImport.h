#pragma once
#include "Module.h"
#include "Globals.h"

#include "ModelData.h"
//#include "GameObject.h"
#include <vector>
#include "Component.h"
#include "CMaterial.h"
#include "Assimp/include/scene.h"
#include "Resource.h"
#include "MeshData.h"
class Component;

class ModuleImport : public Module
{
	enum class FileType {
		NONE,
		MODEL,
		TEXTURE,
	};

public:
	ModuleImport(Application* app, bool start_enabled = true);
	~ModuleImport();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void GetFile(const char* path);
	ModelData* LoadFile(const char* path, Resource::Types type);
	void GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes);
	void GetObjectResources(const aiScene* scene, const char* name);
	//MeshData* GetMeshData(MeshData * meshData, aiMesh* mesh, const aiScene * scene);
	MeshData* GetMeshDataObj(MeshData* meshData, aiMesh* mesh, const aiScene* scene, GameObject* obj);
	void ReleaseFile(const aiScene* scene);
	void ImportModelResources(const aiScene* scene, ModelData* model);

	//Textures
	void LoadCheckerTexture();
	
	GLuint ImportTexture(const char* path);

	//std::vector<GameObject*>tgo;

	
	//Component::Types type;
	
};
