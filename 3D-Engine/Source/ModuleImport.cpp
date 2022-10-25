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
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"



ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImport::~ModuleImport()
{

}

bool ModuleImport::Init()
{
	//Assimp Logs
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LoadCheckerTexture();
	

	return true;
}

void ModuleImport::LoadCheckerTexture() {

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
	}
		
}

void ModuleImport::GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes){

	//std::vector<MeshData*> meshesVec;
	for (int i = 0; i < scene->mNumMeshes; i++) {
		meshes->push_back(GetMeshData(scene->mMeshes[i]));
	}
}


void ModuleImport::ReleaseFile(const aiScene* scene) {
	aiReleaseImport(scene);
}

MeshData* ModuleImport::GetMeshData(aiMesh* mesh) {

	MeshData *meshData = new MeshData();

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
			else {
				memcpy(&meshData->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	//copy texture
	if (mesh->HasTextureCoords(0)) {
		meshData->num_textures = mesh->mNumVertices;
		meshData->textures = new float[mesh->mNumVertices * 2];

		for (unsigned int i = 0; i < meshData->num_textures; i++) {
			meshData->textures[i * 2] = mesh->mTextureCoords[0][i].x;
			meshData->textures[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}
	}

	return meshData;
}