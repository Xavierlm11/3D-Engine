#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "PhysBody3D.h"
#include "CPhysics.h"
#include "PhysVehicle3D.h"

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

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
	bool SaveSceneAtPlay();
	bool LoadScene();
	bool LoadSceneAtPlay();

	void LoadSpecific(uint id);

	void CleanMeshes(std::vector<MeshData*>* meshes);

	GameObject* CreateGO( const char* name, GameObject* parent);
	void CreateGORoot(GameObject* obj, const char* name, GameObject* parent);
	void AddGOList(GameObject* obj);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateVehicle();
	void ControlVehicle();

	bool house_loaded = false;
	bool cube_loaded = false;
	bool sphere_loaded = false;
	bool pyramid_loaded = false;
	bool cylinder_loaded = false;

	const aiScene* modelScene;
	MeshData * modelMesh;

	std::vector<ModelData*> models;
	
	GameObject* RootParent = nullptr;

	std::vector <GameObject*> ListGO;

	std::vector <Resource*> resourceList;
	std::vector <ModelData*> modelList;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
};
