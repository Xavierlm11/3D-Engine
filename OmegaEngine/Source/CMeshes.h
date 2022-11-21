#pragma once
#include "Component.h"
//#include "CMaterial.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include"CTransform.h"
class ModuleRenderer3D;
class MeshData;
class Component;
class CMeshes : public Component
{
public:

	CMeshes(GameObject* obj);
	virtual	~CMeshes();

	void Update() override;

	void MeshRenderer();

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	void OnInspector()override;

	bool active = true;

public:
	MeshData* CompMesh = nullptr;
	CMaterial* MeshMat = nullptr;
	//ModuleRenderer3D* GOmesh;
	
};

