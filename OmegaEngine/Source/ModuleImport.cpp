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
#include "MeshImporter.h"



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

void ModuleImport::GetFile(const char* file_path) {
	


}

//const aiScene* ModuleImport::LoadFile(const char* file_path) {
//	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
//	
//	if (scene != nullptr && scene->HasMeshes()==true)
//	{
//		// Use scene->mNumMeshes to iterate on scene->mMeshes array
//		LOG("File with path: %s loaded successfully!", file_path);
//		return scene;
//	}
//	else {
//		LOG("Error loading scene % s", file_path);
//		return scene;
//	}
//		
//}

void ModuleImport::ImportModelResources(const aiScene* scene, ModelData* model) {

	//std::vector<MeshData*> meshDatasVec;
	//std::vector<Uint64> materialsVec;

	//uint size = 0;
	//char* buffer = nullptr;

	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = new aiMesh();
			mesh = scene->mMeshes[i];
			
			MeshData* meshData = new MeshData(scene->mMeshes[i]->mName.C_Str());//model->assetPath.c_str());
			
			//Resource* meshResource = new Resource(model->assetName.c_str(), Resource::Types::MESH);
			
			MeshImporter::Import(mesh, meshData);
			
			//buffer = MeshImporter::Save(meshData, size);
			//////char* fileBuffer = nullptr;
			//////uint size = 0;
			//////MeshImporter::Save(meshData, size);// &fileBuffer);
			//////if (fileBuffer != nullptr) {
			//////	delete[] fileBuffer;
			//////	fileBuffer = nullptr;
			//////}
			
			//MeshImporter::Load(fileBuffer, meshData);

			

			//meshDatasVec.push_back(meshData->assetID);
			//dobj->GOmesh->meshData = GetMeshDataObj(dobj->GOmesh->meshData, scene->mMeshes[i], scene, dobj);
			model->meshDatas.push_back(meshData);
		}
	}

	if (scene->HasMaterials()) {

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];

			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

			if (numTextures > 0) {

				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				
				const char* filePath = path.C_Str();
				LOG("PATH: %s", filePath);
				
				std::string assetsPath = ASSETS_PATH;
				std::string finalPath = assetsPath + filePath;
				LOG("PATH: %s", finalPath.c_str());

				ImportTexture(finalPath.c_str());
				model->materialDatas.push_back(new MaterialData(finalPath.c_str()));
			}
		}

		if (scene->HasMeshes())
		{
			for (unsigned int j = 0; j < scene->mNumMeshes; j++) {
				//If the mesh has a material assigned
				if (scene->mMeshes[j]->mMaterialIndex != -1) {

					aiMaterial* meshMat = scene->mMaterials[scene->mMeshes[j]->mMaterialIndex];

					aiString path;
					meshMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					const char* filePath = path.C_Str();
					LOG("PATH: %s", filePath);

					std::string assetsPath = ASSETS_PATH;
					std::string finalPath = assetsPath + filePath;
					LOG("PATH: %s", finalPath.c_str());

					for (unsigned int k = 0; k < model->materialDatas.size(); k++) {

						//If material of mesh is the same as one of the model materials list
						if (finalPath == model->materialDatas[k]->assetPath) {

							for (unsigned int l = 0; l < model->meshDatas.size(); l++) {

								//Find the mesh in model meshes list
								if (model->meshDatas[l]->assetName == scene->mMeshes[j]->mName.C_Str()) 
								{
									//Now we need to find the game object of the mesh data and assign it the texture
									//model->meshDatas[l]
								}

							}


						}
					}
				}
			}
		}
		

	}
}

ModelData* ModuleImport::LoadFile(const char* file_path, Resource::Types type) {

	//Resource* new_res = new Resource(file_path, type);
	

	if (type==Resource::Types::MODEL) {
		const aiScene* new_scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

		ModelData* new_model = new ModelData(file_path);
		ImportModelResources(new_scene, new_model);
		//App->scene->modelList.push_back(new_model);
		App->scene->resourceList.push_back(new_model);

		
		ReleaseFile(new_scene);

		return new_model;
	}

	
	
}
//
//void ModuleImport::GetMeshDatas(const aiScene* scene, std::vector<MeshData*>* meshes){
//
//	for (int i = 0; i < scene->mNumMeshes; i++) {
//		MeshData* newMesh = new MeshData();
//		
//		//GetMeshData(newMesh, scene->mMeshes[i], scene);
//		newMesh->LoadBuffers();
//		meshes->push_back(newMesh);
//	}
//}

void ModuleImport::GetObjectResources(const aiScene* scene, const char* name) {

	GameObject* parent = nullptr;
	parent = App->scene->RootParent;

	std::vector<Uint64> meshesVec;
	std::vector<Uint64> materialsVec;

	//if (scene->HasMeshes())
	//{
	//	if (scene->mNumMeshes > 1)
	//	{
	//		GameObject* dobj = nullptr;
	//		dobj = App->scene->CreateGO(name, parent);
	//		parent = dobj;
	//	}

	//	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	//	{
	//		GameObject* dobj = nullptr;
	//		dobj = App->scene->CreateGO(scene->mMeshes[i]->mName.C_Str(), parent);


	//		MeshData* newMesh = new MeshData();

	//		dobj->CreateComp(Component::Types::MESH);

	//		dobj->GOmesh->meshData = new MeshData();
	//		dobj->GOmesh->meshData = GetMeshDataObj(dobj->GOmesh->meshData, scene->mMeshes[i], scene, dobj);
	//		dobj->GOmesh->meshData->LoadBuffers();

	//	}
	//}

	if (scene->HasMaterials()) {
		
		//for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		//{
		//	aiMaterial* material = new aiMaterial();
		//	material = scene->mMaterials[mesh->mMaterialIndex];

		//}

		//

		//uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		//obj->CreateComp(Component::Types::MATERIAL);
		////obj->GOmat->materialData->texture_id

		//obj->GOmat->materialData = new MaterialData();

		//if (numTextures > 0) {

		//	aiString path;
		//	material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		//	LOG("PATH: %s", path.C_Str());
		//	std::string assetsPath = "Assets/";
		//	const char* filePath = path.C_Str();

		//	std::string finalPath = assetsPath + filePath;
		//	LOG("PATH: %s", finalPath.c_str());
		//	//const char* newPath = "Assets/%s"

		//	obj->GOmat->materialData->texture_id = ImportTexture(finalPath.c_str());
		//}
		//

	}

	parent = nullptr;
	//tgo.clear();
}

void ModuleImport::ReleaseFile(const aiScene* scene) {
	aiReleaseImport(scene);
}


//
//MeshData* ModuleImport::GetMeshData(MeshData * meshData, aiMesh* mesh, const aiScene * scene) {
//
//
//	aiMaterial* material = new aiMaterial();
//	material = scene->mMaterials[mesh->mMaterialIndex];
//
//	if (scene->HasMaterials()) {
//		uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
//		
//
//		if (numTextures > 0) {
//
//			aiString path;
//			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
//
//			LOG("PATH: %s", path.C_Str());
//			std::string assetsPath = "Assets/";
//			const char* filePath = path.C_Str();
//
//			std::string finalPath = assetsPath + filePath;
//			LOG("PATH: %s", finalPath.c_str());
//			//const char* newPath = "Assets/%s"
//			meshData->material = new MaterialData();
//			meshData->material->texture_id = App->imp->ImportTexture(finalPath.c_str());
//		}
//	}
//	
//	
//	
//
//	
//	
//	
//
//	//copy vertices
//	meshData->num_vertices = mesh->mNumVertices;
//	meshData->vertices = new float[meshData->num_vertices * 3];
//
//	memcpy(meshData->vertices, mesh->mVertices, sizeof(float) * meshData->num_vertices * 3);
//
//	LOG("New mesh with %d vertices", meshData->num_vertices);
//
//	//copy faces
//	if (mesh->HasFaces())
//	{
//		meshData->num_indices = mesh->mNumFaces * 3;
//		meshData->indices = new uint[meshData->num_indices]; // assume each face is a triangle
//		for (uint i = 0; i < mesh->mNumFaces; ++i)
//		{
//			if (mesh->mFaces[i].mNumIndices != 3) {
//				LOG("WARNING, geometry face with != 3 indices!");
//			}
//			else 
//			{
//				memcpy(&meshData->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
//			}
//		}
//	}
//	if (mesh->mNumUVComponents > 0) {
//		LOG("HAS TEX");
//	}
//	else {
//		LOG("NO TEX");
//	}
//	//copy texture coords
//	if (mesh->HasTextureCoords(0)) {
//		meshData->num_textureCoords = mesh->mNumVertices;
//		meshData->textureCoords = new float[mesh->mNumVertices * 2];
//
//		for (unsigned int i = 0; i < meshData->num_textureCoords; i++) {
//			meshData->textureCoords[i * 2] = mesh->mTextureCoords[0][i].x;
//			meshData->textureCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
//		}
//	}
//	
//	
//
//	return meshData;
//}

MeshData* ModuleImport::GetMeshDataObj(MeshData* meshData, aiMesh* mesh, const aiScene* scene, GameObject* obj) {


	aiMaterial* material = new aiMaterial();
	material = scene->mMaterials[mesh->mMaterialIndex];

	//if (scene->HasMaterials()) {
	//	uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
	//	obj->CreateComp(Component::Types::MATERIAL);
	//	//obj->GOmat->materialData->texture_id
	//	
	//	obj->GOmat->materialData = new MaterialData();

	//	if (numTextures > 0) {

	//		aiString path;
	//		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	//		LOG("PATH: %s", path.C_Str());
	//		std::string assetsPath = "Assets/";
	//		const char* filePath = path.C_Str();

	//		std::string finalPath = assetsPath + filePath;
	//		LOG("PATH: %s", finalPath.c_str());
	//		//const char* newPath = "Assets/%s"

	//		obj->GOmat->materialData->texture_id = ImportTexture(finalPath.c_str());
	//	}
	//}








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