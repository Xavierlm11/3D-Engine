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

void GameObject::SetParent(GameObject* newparent)
{
	GameObject* p = newparent;
	if (p==parent)return;
	
	if (p->parent)
	{
		p->parent->DeleteChild(p);
	}
	parent = p;

	if (p)
	{
		p->childrens.push_back(p);
	}
	//set transform

}

void GameObject::DeleteChild(GameObject* delchild)
{
	for (uint i=0;i<childrens.size();++i)
	{
		if (childrens[i]==delchild)
		{
			childrens.erase(childrens.begin() + i);
		}

	}
}

void GameObject::Enable()
{
	active = true;
}

void GameObject::Disable()
{
	active = false;
}

bool GameObject::IsEnable()
{
	return active;
}

