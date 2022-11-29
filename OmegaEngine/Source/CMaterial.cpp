#include "CMaterial.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Application.h"

CMaterial::CMaterial(GameObject* obj):Component(obj, Types::MATERIAL)
{
	
}

CMaterial::~CMaterial()
{
	materialData = nullptr;
	delete materialData;

}

void CMaterial::Update()
{
	

}

MaterialData* CMaterial::GetMaterial()
{
	//mat
	return materialData;
}

