#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleScene : public Module
{
public:

	enum class Shapes {
		NONE,
		HOUSE,
		CUBE,
		SPHERE,
		PYRAMID,
		CYLINDER
	};

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool SaveScene();
	bool LoadScene();

	void LoadCustom(const char* path, std::vector<MeshData*>* meshes);
	void LoadHouse(std::vector<MeshData*>* meshes);
	void LoadCube(std::vector<MeshData*>* meshes);
	void LoadSphere(std::vector<MeshData*>* meshes);
	void LoadPyramid(std::vector<MeshData*>* meshes);
	void LoadCylinder(std::vector<MeshData*>* meshes);

	void CleanMeshes(std::vector<MeshData*>* meshes);

	bool house_loaded = false;
	bool cube_loaded = false;
	bool sphere_loaded = false;
	bool pyramid_loaded = false;
	bool cylinder_loaded = false;

	const aiScene* modelScene;
	MeshData * modelMesh;

	std::vector<ModelData*> models;
	
	
};
