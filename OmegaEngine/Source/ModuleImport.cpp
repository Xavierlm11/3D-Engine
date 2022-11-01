#include "Globals.h"
#include "Application.h"
#include "ModuleImport.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment( lib, "DevIL/libx86/DevIL.lib" )
#pragma comment( lib, "DevIL/libx86/ILU.lib" )
#pragma comment( lib, "DevIL/libx86/ILUT.lib" )
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "DevIL/include/il.h"

#include <iostream>


ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImport::~ModuleImport()
{

}

bool ModuleImport::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	//Assimp Logs
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LoadCheckerTexture();

	return true;
}

void ModuleImport::LoadCheckerTexture() {

}

GLuint ModuleImport::ImportTexture(const char* path) {
	
	ILubyte* Lump;
	ILuint Size;
	FILE* File;

	File = fopen(path, "rb");
	if (File != NULL) {
		
		fseek(File, 0, SEEK_END);
		Size = ftell(File);

		Lump = (ILubyte*)malloc(Size);
		fseek(File, 0, SEEK_SET);
		fread(Lump, 1, Size, File);

		LOG("SIZE: %i", Size);

		if (ilLoadImage(path) == true) {

			LOG("FOUND");
			GLuint texID;
			GLuint texture;


			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			texID = ilutGLBindTexImage();

			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			ilDeleteImages(1, &texID);

			glBindTexture(GL_TEXTURE_2D, 0);
			fclose(File);
			free(Lump);

			return texID;
		}

		fclose(File);


		LOG("NOT FOUND");
	}

}

update_status ModuleImport::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

const aiScene* ModuleImport::LoadFile(const char* file_path) {
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes()==true)
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		LOG("File with path: %s loaded successfully!", file_path);
		return scene;
	}
	else {
		LOG("Error loading scene % s", file_path);
		return scene;
	}
		
}

void ModuleImport::GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes){

	for (int i = 0; i < scene->mNumMeshes; i++) {
		MeshData* newMesh = new MeshData();
		
		GetMeshData(newMesh, scene->mMeshes[i], scene);
		newMesh->LoadBuffers();
		meshes->push_back(newMesh);
	}
}

void ModuleImport::GetMeshDatasObj(const aiScene* scene ,const char* name) {

	GameObject* parent = nullptr;
	parent = App->scene->RootParent;

	int j = 0;
	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			MeshData* newMesh = new MeshData();

			GameObject* dobj = nullptr;

			/*if(scene->mNumMeshes>1)

			*/
			/*if (scene->mRootNode->mParent  ) {
				dobj = App->scene->CreateGO("picaporte", App->scene->RootParent);
				tgo.push_back(dobj);
			}*/
			//scene->mRootNode.mp
			if (scene->mNumMeshes> 1 && i == 0)
			{
				dobj = App->scene->CreateGO("picaporton", parent);
				parent = dobj;
			}

			if (scene->mRootNode->mParent==nullptr && scene->mRootNode->mChildren[0]->mNumChildren!=0) {
				dobj = App->scene->CreateGO("picaporte", parent);
				//tgo.push_back(dobj);
				parent = dobj;
			}
			else if (scene->mRootNode->mChildren!=nullptr) {
				dobj = App->scene->CreateGO("picaportito", parent);
				//tgo.push_back(dobj);
				//parent = dobj;
			}
		//	if (i <= 0) {
		//		dobj = App->scene->CreateGO(name, App->scene->RootParent);
		//		tgo.push_back(dobj);
		//		++j;
		//	}
		//	//scene->mRootNode.m///////////aquuiiiiiiiiiiiiiiiii
		////App->scene->RootParent->childrens.push_back(dobj);
		//	if (i > 0)
		//	{
		//		dobj = App->scene->CreateGO(name, tgo[0]);

		//	}
			dobj->CreateComp(Component::Types::MESH);
			dobj->GOmesh->CompMesh = new MeshData();
			dobj->GOmesh->CompMesh = GetMeshData(dobj->GOmesh->CompMesh, scene->mMeshes[i], scene);
			dobj->GOmesh->CompMesh->LoadBuffers();


			//meshes->push_back(newMesh);

		}
	}
	else {
		LOG("IMPOSIBLE TO LOAD THIS OBJECT");
	}

	parent = nullptr;
	tgo.clear();
}

void ModuleImport::ReleaseFile(const aiScene* scene) {
	aiReleaseImport(scene);
}



MeshData* ModuleImport::GetMeshData(MeshData * meshData, aiMesh* mesh, const aiScene * scene) {


	aiMaterial* material = new aiMaterial();
	material = scene->mMaterials[mesh->mMaterialIndex];

	if (scene->HasMaterials()) {
		uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		

		if (numTextures > 0) {

			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			LOG("PATH: %s", path.C_Str());
			std::string assetsPath = "Assets/";
			const char* filePath = path.C_Str();

			std::string finalPath = assetsPath + filePath;
			LOG("PATH: %s", finalPath.c_str());
			//const char* newPath = "Assets/%s"
			meshData->material = new MaterialData();
			meshData->material->texture_id = App->imp->ImportTexture(finalPath.c_str());
		}
	}
	
	
	

	
	
	

	//copy vertices
	meshData->num_vertices = mesh->mNumVertices;
	meshData->vertices = new float[meshData->num_vertices * 3];

	memcpy(meshData->vertices, mesh->mVertices, sizeof(float) * meshData->num_vertices * 3);

	LOG("New mesh with %d vertices", meshData->num_vertices);

	//copy faces
	if (mesh->HasFaces())
	{
		meshData->num_indices = mesh->mNumFaces * 3;
		meshData->indices = new uint[meshData->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else 
			{
				memcpy(&meshData->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	if (mesh->mNumUVComponents > 0) {
		LOG("HAS TEX");
	}
	else {
		LOG("NO TEX");
	}
	//copy texture coords
	if (mesh->HasTextureCoords(0)) {
		meshData->num_textureCoords = mesh->mNumVertices;
		meshData->textureCoords = new float[mesh->mNumVertices * 2];

		for (unsigned int i = 0; i < meshData->num_textureCoords; i++) {
			meshData->textureCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			meshData->textureCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}
	}
	
	

	return meshData;
}