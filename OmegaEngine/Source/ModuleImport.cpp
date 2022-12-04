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
#include "ModelImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"


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

vector<string> ModuleImport:: GetFilesInFolder(string folder)
{
	vector<string> names;
	char search_path[200];
	sprintf(search_path, "%s*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				string temp;
				temp = fd.cFileName;
				const char *filename = temp.c_str();
				char fullFilename[MAX_PATH];

				GetFullPathName(filename, MAX_PATH, fullFilename, nullptr);
				string temp2 = fullFilename;
				string filepath_s = temp2.substr(0,temp2.find_last_of('\\') + 1) + "Assets\\";
				filepath_s += temp;
				names.push_back(filepath_s);
			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
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


void ModuleImport::ImportModelResources(const aiScene* scene, ModelData* model) {

	
	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = new aiMesh();
			mesh = scene->mMeshes[i];
			
			MeshData* meshData = new MeshData(scene->mMeshes[i]->mName.C_Str());

			MeshImporter::Import(mesh, meshData);
			
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
				LOG("File path: %s", filePath);
				
				std::string assetsPath = ASSETS_PATH;
				std::string finalPath = assetsPath + filePath;
				LOG("Assets path: %s", finalPath.c_str());

				ImportTexture(finalPath.c_str());

				MaterialData* mat = new MaterialData(finalPath.c_str());
				model->materialDatas.push_back(mat);
			}
		}

		
		

	}
}

update_status ModuleImport::Update(float dt) {

	if (hasToLoadAssets==false) {

		const char* folder_name = ASSETS_PATH;
		vector<string> assetFilesVec = App->imp->GetFilesInFolder(folder_name);

		for (int i = 0; i < assetFilesVec.size(); i++) {
			ImportAsset(assetFilesVec[i].c_str());
		}

		for (int i = 0; i < App->scene->resourceList.size(); i++) {

			if (App->scene->resourceList[i]->resourceType == Resource::Types::MODEL) {
				GetMaterialsID(assetFilesVec[i].c_str(), (ModelData*)App->scene->resourceList[i]);
			}
		}

		hasToLoadAssets = true;


		for (int i = 0; i < App->scene->resourceList.size(); i++) {
			if (App->scene->resourceList[i]->assetName == "street2") {
				App->scene->LoadSpecific(App->scene->resourceList[i]->assetID);
			}
			
		}
	}

	

	return update_status::UPDATE_CONTINUE;
}

void ModuleImport::ImportAsset(const char* filePath) {


	std::string dropped_filedir_s = filePath;
	std::string fileName_s = dropped_filedir_s.substr(dropped_filedir_s.find_last_of('\\') + 1);
	std::string extension_s = fileName_s.substr(fileName_s.find_last_of('.') + 1);
	std::string assetsPath_s = ASSETS_PATH;

	const char* fileName = fileName_s.c_str();

	std::string finalAssetPath = assetsPath_s + fileName;

	LOG("PATH: %s", finalAssetPath.c_str());

	if (extension_s == "png" || extension_s == "PNG")
	{
		MaterialData* new_material_data = (MaterialData*)App->imp->LoadFile(filePath, Resource::Types::MATERIAL);
		App->fileSystem->ImportFileToDir(dropped_filedir_s.c_str(), assetsPath_s.c_str());

		std::string libraryPath_s = LIB_MATERIAL_PATH;

		char* buffer = nullptr;

		uint bufferSize = App->fileSystem->FileToBuffer(fileName_s.c_str(), &buffer);

		MaterialImporter::LoadTextureLump(buffer, bufferSize);
		bufferSize = MaterialImporter::Save(&buffer);

		std::string new_name = std::to_string(new_material_data->assetID) + ".chad";
		std::string finalLibraryPath = libraryPath_s + new_name;
		App->fileSystem->SaveFile(finalLibraryPath.c_str(), buffer, bufferSize);

		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}

	}
	else if (extension_s == "fbx" || extension_s == "FBX")
	{
		ModelData* new_model_data = (ModelData*)App->imp->LoadFile(filePath, Resource::Types::MODEL);
		App->fileSystem->ImportFileToDir(dropped_filedir_s.c_str(), assetsPath_s.c_str());

		std::string libraryPath_s = LIB_MESH_PATH;


		uint size = 0;
		char* buffer = nullptr;
		buffer = ModelImporter::Save(new_model_data, size);


		std::string new_name = std::to_string(new_model_data->assetID) + ".chad";
		if (firstID == 0)
		{
			firstID = new_model_data->assetID;
		}
		
		std::string finalLibraryPath = libraryPath_s + new_name;
		App->fileSystem->SaveFile(finalLibraryPath.c_str(), buffer, size);

		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}

	}
}

Resource* ModuleImport::LoadFile(const char* file_path, Resource::Types type) {
	
	const aiScene* new_scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	switch(type) {
		case Resource::Types::MODEL:
		{
			ModelData* new_model = new ModelData(file_path);
			ImportModelResources(new_scene, new_model);
			
			App->scene->resourceList.push_back(new_model);

			return new_model;
			break;
		}
		case Resource::Types::MATERIAL:
		{
			MaterialData* new_material = new MaterialData(file_path);

			App->scene->resourceList.push_back(new_material);

			return new_material;
			break;
		}
		default: 
		{
			ReleaseFile(new_scene);
			break;
		}
			
			
	}
}

void ModuleImport::GetMaterialsID(const char* path, ModelData* model) {

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene->HasMaterials())
	{
		if (scene->HasMeshes())
		{
			for (unsigned int j = 0; j < scene->mNumMeshes; j++) {

				//If the mesh has a material assigned
				if (scene->mMeshes[j]->mMaterialIndex != -1) {

					aiMaterial* meshMat = scene->mMaterials[scene->mMeshes[j]->mMaterialIndex];

					aiString path;
					meshMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					const char* filePath = path.C_Str();
					LOG("File path: %s", filePath);

					std::string assetsPath = ASSETS_PATH;
					std::string finalPath = assetsPath + filePath;
					LOG("Assets path: %s", finalPath.c_str());

					for (unsigned int k = 0; k < model->materialDatas.size(); k++) {

						for (unsigned int l = 0; l < App->scene->resourceList.size(); l++) {

							if (App->scene->resourceList[l]->resourceType == Resource::Types::MATERIAL) {

								if (model->materialDatas[k]->assetName == ((MaterialData*)App->scene->resourceList[l])->assetName) {

									if (App->scene->resourceList[l]->fileName == filePath) {

										for (unsigned int m = 0; m < model->meshDatas.size(); m++) {

											//Find the mesh in model meshes list
											if (model->meshDatas[m]->assetName == scene->mMeshes[j]->mName.C_Str())
											{
												//Now we need to find the game object of the mesh data and assign it the texture
												model->meshDatas[m]->materialAttachedID = App->scene->resourceList[l]->assetID;
											}

										}
									}
								}
							}
							
						}
					}
				}
			}
		}
	}

	ReleaseFile(scene);

}

void ModuleImport::GetObjectResources(const aiScene* scene, const char* name) {

	GameObject* parent = nullptr;
	parent = App->scene->RootParent;

	std::vector<Uint64> meshesVec;
	std::vector<Uint64> materialsVec;

	parent = nullptr;

}

void ModuleImport::ReleaseFile(const aiScene* scene) {
	aiReleaseImport(scene);
}


MeshData* ModuleImport::GetMeshDataObj(MeshData* meshData, aiMesh* mesh, const aiScene* scene, GameObject* obj) {


	aiMaterial* material = new aiMaterial();
	material = scene->mMaterials[mesh->mMaterialIndex];


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