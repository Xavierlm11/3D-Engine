#include "GameObject.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTransform.h"

GameObject::GameObject(const char* name, GameObject* parent):name(name)
{
	if(parent!=nullptr )
	{ 
		while(uid!=0)uid = External->rand.Int();
		//parent->childrens.push_back(this);
	}
	SetParent(parent);

	CreateComp(Component::Types::TRANSFORM);
}

void GameObject::SetProperties() {

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
	for (uint i = 0; i < children.size(); ++i)
	{
		delete children[i];
		children[i] = nullptr;
	}
	children.clear();

	parent = nullptr;
	delete parent;

	GOmat = nullptr;
	delete GOmat;

	GOtrans = nullptr;
	delete GOtrans;
	GOmesh = nullptr;
	delete GOmesh;
	//External->CleanVec(childrens);
}

void GameObject::SetParent(GameObject* newparent)
{
	GameObject* p = newparent;
	if (newparent ==parent)
		return ;
	
	if (newparent->parent)
	{
		newparent->parent->DeleteChild(this);
	}
	parent = newparent;

	if (newparent)
	{
		newparent->children.push_back(this);
	}
	//set transform

}

void GameObject::DeleteChild(GameObject* delchild)
{
	for (uint i=0;i<children.size();++i)
	{
		if (children[i]==delchild)
		{
			children.erase(children.begin() + i);
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

Component* GameObject::GetComponent(Component::Types stype)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == stype)
		{
			return components[i];
		}
		
	}
	return nullptr;
}

GameObject* GameObject::GetParent()
{
	return parent;
}

std::vector<GameObject*> GameObject::GetChildrens()
{
	return children;
}

int GameObject::GetLayer() {
	int deepLayer = 0;
	bool hasParent = true;
	while (hasParent == true) {
		if (parent != nullptr) {
			deepLayer++;
		}
		else {
			hasParent = false;
		}
	}
	return deepLayer;
}

Component* GameObject::CreateComp(Component::Types type)
{
	Component* comp = nullptr;
	switch (type)
	{
		case Component::Types::NONE :
		{
			//SI
		}
			break;
		case Component::Types::TRANSFORM:
		{
			CTransform* ctrans = new CTransform(this);
			GOtrans = ctrans;
			comp = ctrans;
		}
			break;
		case Component::Types::MATERIAL:
		{
			//comp = new CMaterial(this);
			CMaterial* cmat= new CMaterial(this);
			GOmat = cmat;
			comp = cmat;
		}
			break;
		case Component::Types::MESH:
		{
			CMesh * cmesh = new CMesh(this);
			GOmesh = cmesh;
			comp = cmesh;
			//GOmesh = (CMeshes*)comp;
		}
			break;
		case Component::Types::LIGHT:
		{
			//no
		}
			break;
	}
	if (comp != nullptr) components.push_back(comp);
	
	return comp;
}

void GameObject::Update(float dt)
{

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsEnable())
		{

			components[i]->Update();
			
		}

	}

}

void GameObject::Editor()
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsEnable())
		{

			components[i]->OnInspector();

		}

	}
}


void GameObject::Remove()
{
	if (parent != nullptr) {
		if (!components.empty())
		{
			for (uint i = 0; i < components.size(); ++i)
			{
				delete components[i];
				components[i] = nullptr;
			}
			components.clear();
		}
		//External->CleanVec(components);
		if (!children.empty()) {
			for (uint i = 0; i < children.size(); ++i)
			{
				delete children[i];
				children[i] = nullptr;
			}
			children.clear();
		}
		parent = nullptr;
		delete parent;

		GOmat = nullptr;
		delete GOmat;

		GOtrans = nullptr;
		delete GOtrans;
		GOmesh = nullptr;
		delete GOmesh;
		
		

	}

}
