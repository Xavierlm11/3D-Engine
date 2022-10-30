#include "CMeshes.h"
#include "CMaterial.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Source/External/Glew/libx86/glew32.lib")

#include "Primitive.h"

CMeshes::CMeshes(GameObject* obj) :Component(obj, Types::MESH)
{
}
CMeshes::~CMeshes()
{
}

void CMeshes::Update()
{

}

void CMeshes::MeshRenderer()
{


	GOmesh->DrawMesh();


	
}
