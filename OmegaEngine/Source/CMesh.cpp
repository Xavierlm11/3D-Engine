#include "CMesh.h"
#include "CMaterial.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"
#include "Application.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Source/External/Glew/libx86/glew32.lib")

#include "Primitive.h"

CMesh::CMesh(GameObject* obj) :Component(obj, Types::MESH)
{

}
CMesh::~CMesh()
{

	CompMesh->UnloadMesh();
	CompMesh = nullptr;
	delete CompMesh;

	MeshMat = nullptr;
	delete MeshMat;

}

void CMesh::Update()
{
	if(CompMesh!=nullptr)
		External->renderer3D->meshlist.push_back(this);
		
	
}

void CMesh::MeshRenderer()
{
	//External->rendere
	if (GO->GOmat->GetMaterial() != nullptr && External->renderer3D->mode == ModuleRenderer3D::RenderMode::NORMAL) {
		CompMesh->DrawMesh(GO->GOmat->GetMaterial()->texture_id, GO->GOtrans->matrix);

		
	}
	else if (External->renderer3D->mode== ModuleRenderer3D::RenderMode::CHECKERS)
	{
		CompMesh->DrawMesh(External->renderer3D->checkersID, GO->GOtrans->matrix);
	}
	else {

		CompMesh->DrawMesh(0, GO->GOtrans->matrix);
	}
	
	
}

void CMesh::OnInspector()
{
}
