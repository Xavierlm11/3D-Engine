#pragma once
#include "Component.h"
class ModuleRenderer3D;
class MeshData;
class CMeshes : public Component
{
public:

	CMeshes(GameObject* obj);
	virtual	~CMeshes();

	void Update() override;

	void MeshRenderer();

public:
	MeshData* GOmesh;
	//ModuleRenderer3D* GOmesh;
	
};

