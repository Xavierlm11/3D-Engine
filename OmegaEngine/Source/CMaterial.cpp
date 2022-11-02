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
	CmMat = nullptr;
	delete CmMat;

}

void CMaterial::Update()
{
	

}

MaterialData* CMaterial::GetMaterial()
{
	//mat
	return CmMat;
}

