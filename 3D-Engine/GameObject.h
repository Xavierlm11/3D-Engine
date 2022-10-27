#pragma once
#include <string>
#include <vector>
#include <stdio.h>	
#include <list>
#include <math.h>
#include "Source/Globals.h"
#include "Source/Application.h"

class Component;

class GameObject
{
public:
	GameObject(const char* name, GameObject* parent);
	virtual ~GameObject();

public:

	std::string name;

	std::vector<GameObject*> childrens;
	std::vector<Component*> components;

	uint uid=0;

private:

};

