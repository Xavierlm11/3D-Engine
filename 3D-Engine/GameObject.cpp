#include "GameObject.h"


GameObject::GameObject(const char* name, GameObject* parent):name(name)
{
	uid = External->rand.Int();
	if(parent!=nullptr )
	{ 
		parent->childrens.push_back(parent);
	}
}

GameObject::~GameObject() 
{
	for (uint i=0; i<components.size();++i)
	{
		delete components[i];
		components[i] = nullptr;
	}
	components.clear();
	//External->CleanVec(components);
	for (uint i = 0; i < childrens.size(); ++i)
	{
		delete childrens[i];
		childrens[i] = nullptr;
	}
	childrens.clear();

	//External->CleanVec(childrens);
}

void GameObject::Set
