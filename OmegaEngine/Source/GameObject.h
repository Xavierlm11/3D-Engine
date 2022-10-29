#pragma once
#include <string>
#include <vector>
#include <stdio.h>	
#include <list>
#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "Component.h"
class Component;

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

	Component* GetComponent(Component::Types stype);

	GameObject* GetParent();

	std::vector<GameObject*> GetChildrens();

	Component* CreateComp(Component::Types type);

public:

	std::string name;

	std::vector<GameObject*> childrens;
	std::vector<Component*> components;

	GameObject* parent = nullptr;

	uint uid=0;

public:
	bool active=true;
	bool visible=false;
};

