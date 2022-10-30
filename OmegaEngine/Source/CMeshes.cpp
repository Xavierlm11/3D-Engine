#include "CMeshes.h"
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

CMeshes::CMeshes(GameObject* obj) :Component(obj, Types::MESH)
{
}
CMeshes::~CMeshes()
{
}

void CMeshes::Update()
{
	if(GOmesh!=nullptr)
		External->renderer3D->meshlist.push_back(this);
		
	//MeshRenderer();
}

void CMeshes::MeshRenderer()
{
	/*
	switch (External->renderer3D->mode)
	{
	case ModuleRenderer3D::RenderMode::NONE:

		break;
	case ModuleRenderer3D::RenderMode::NORMAL:
		//p.Render();
		for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
		{
			//if (App->scene->models[0]->meshes[i]->id_textureCoords != 0) {
			//	//App->scene->models[0]->meshes[i]->DrawMesh(App->scene->models[0]->meshes[i]->id_textures);
			//}

			//if (App->scene->models[0]->meshes[i]->num_textureCoords > 0) {
			//	//App->scene->models[0]->meshes[i]->DrawMesh(houseTexID);
			//}
			//else {
			//	//App->scene->models[0]->meshes[i]->DrawMesh(checkersID);
			//}
			//App->scene->models[0]->meshes[i]->DrawMesh(houseTexID);
			//App->scene->models[0]->meshes[i]->DrawMesh(checkersID);
			if (GOmesh->material != nullptr) {
				GOmesh->DrawMesh(GOmesh->material->texture_id);
			}
			else {

				GOmesh->DrawMesh(0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			//LOG("ID_TEX: %i", App->scene->meshes[i]->id_textures);
		//}
		//DrawCube();
		break;
	case ModuleRenderer3D::RenderMode::WIREFRAME:
		//p.Render();
		//for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
		//{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (GOmesh->material != nullptr) {
				GOmesh->DrawMesh(GOmesh->material->texture_id);
			}
			else {

				GOmesh->DrawMesh(0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//}
		//Draw();
		break;
	}
	*/
	//GOmesh->DrawMesh(0);

	if (GOmesh->material != nullptr) {
		GOmesh->DrawMesh(GOmesh->material->texture_id);
	}
	else {

		GOmesh->DrawMesh(0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
}
