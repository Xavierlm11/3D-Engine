#pragma once
#include <string>
#include <vector>
#include <stdio.h>	
#include <list>
#include <math.h>
#include "Source/Globals.h"
#include "Source/Application.h"
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

