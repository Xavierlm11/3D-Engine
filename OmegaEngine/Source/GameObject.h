#pragma once
#include <string>
#include <vector>
#include <stdio.h>	
#include <list>
#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "PhysBody3D.h"

class Component;

class CMesh;

class CMaterial;

class CTransform;

class CCamera;

class CPhysics;

class GameObject
{
public:
	GameObject(const char* name, GameObject* parent);
	virtual ~GameObject();

	void SetParent(GameObject* newparent);

	void DeleteChild(GameObject* delchild);

	void Enable();

	void Disable();

	bool IsEnable();

	void SetProperties();

	Component* GetComponent(Component::Types stype);

	GameObject* GetParent();

	std::vector<GameObject*> GetChildrens();

	Component* CreateComp(Component::Types type);
	void DeleteComponent();
	void Update(float dt);

	void Editor();

	void Remove();
	
	int GetLayer();

public:

	std::string name;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

	GameObject* parent = nullptr;

	CMesh* GOmesh=nullptr;
	CMaterial* GOmat = nullptr;
	CTransform* GOtrans = nullptr;
	CCamera* GOcam = nullptr;
	CPhysics* GOphys = nullptr;
	uint uid=0;

public:
	bool active=true;
	bool visible=false;
	bool showingInHierarchy = true;
	int layer = 0;
};

