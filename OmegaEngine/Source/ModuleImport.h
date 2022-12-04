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

using namespace std;
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
	update_status Update(float dt);
	bool CleanUp();

	void GetFile(const char* path);
	Resource* LoadFile(const char* path, Resource::Types type);
	void GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes);
	void GetObjectResources(const aiScene* scene, const char* name);

	MeshData* GetMeshDataObj(MeshData* meshData, aiMesh* mesh, const aiScene* scene, GameObject* obj);
	void ReleaseFile(const aiScene* scene);

	void ImportModelResources(const aiScene* scene, ModelData* model);

	void GetMaterialsID(const char* path, ModelData* filePath);

	void ImportAsset(const char* filePath);

	vector<string> GetFilesInFolder(string folder);

	//Textures
	void LoadCheckerTexture();
	
	GLuint ImportTexture(const char* path);

	bool hasToLoadAssets = false;

	uint firstID = 0;
	
};
