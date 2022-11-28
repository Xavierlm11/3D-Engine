#pragma once
#include "Component.h"
//#include "CMaterial.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include"CTransform.h"
#include "MeshData.h"
class ModuleRenderer3D;

class Component;
class CMesh : public Component
{
public:

	CMesh(GameObject* obj);
	virtual	~CMesh();

	void Update() override;

	void MeshRenderer();

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	void OnInspector()override;

	bool active = true;

public:
	MeshData* meshData = nullptr;
	CMaterial* MeshMat = nullptr;
	//ModuleRenderer3D* GOmesh;
	
};

