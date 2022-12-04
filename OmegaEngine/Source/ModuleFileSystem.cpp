#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#include "PhysFS/include/physfs.h"
#include <iostream>
#include "SDL/include/SDL_filesystem.h"

#include <filesystem>

#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleFileSystem::~ModuleFileSystem()
{

}

bool ModuleFileSystem::Init()
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	string folder_name;

	folder_name = ".";

	if (PHYSFS_setWriteDir(folder_name.c_str()) == 0) {
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;

		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}
	
	
	if (PHYSFS_mount(folder_name.c_str(), nullptr, 1) == 0)
	{
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;

		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}

	folder_name = ASSETS_PATH;
	if (PHYSFS_mount(folder_name.c_str(), nullptr, 1) == 0)
	{
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;

		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}
	
	folder_name = LIBRARY_PATH;
	if (PHYSFS_mount(folder_name.c_str(), nullptr, 1) == 0)
	{
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;

		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}

	folder_name = LIB_MESH_PATH;
	if (PHYSFS_mount(folder_name.c_str(), nullptr, 1) == 0)
	{
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;
		
		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}

	folder_name = LIB_MATERIAL_PATH;
	if (PHYSFS_mount(folder_name.c_str(), nullptr, 1) == 0)
	{
		LPSECURITY_ATTRIBUTES attr;
		attr = NULL;

		CreateDirectory(folder_name.c_str(), attr);
		PHYSFS_mount(folder_name.c_str(), nullptr, 1);
	}


	return true;
}


uint ModuleFileSystem::FileToBuffer(const char* filePath, char** fileBuffer) const {

	uint ret = 0;

	PHYSFS_file* pysfsFile = PHYSFS_openRead(filePath);

	if (pysfsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(pysfsFile);

		if (size > 0)
		{
			*fileBuffer = new char[size + 1];
			uint readData = (uint)PHYSFS_read(pysfsFile, *fileBuffer, 1, size);

			if (readData == size)
			{
				ret = readData;
				(*fileBuffer)[size] = '\0';
				
			}
		}

		PHYSFS_close(pysfsFile);
	}

	return ret;
}

uint ModuleFileSystem::ImportFileToDir(const char* filePath, const char* newPath) {

	uint size = 0;

	std::string assetsDir = newPath;
	

	std::FILE* file;
	fopen_s(&file, filePath, "rb");

	if (file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		char* buffer = new char[size];

		size = fread(buffer, 1, size, file);

		if (size > 0)
		{
			std::string dropped_filedir_s = filePath;
			std::string fileName_s = dropped_filedir_s.substr(dropped_filedir_s.find_last_of('\\') + 1);
			std::string totalAssetsDir = assetsDir + fileName_s;
			
			PHYSFS_file* physfs_assets_file = nullptr;
			physfs_assets_file = PHYSFS_openWrite(totalAssetsDir.c_str());
			if (physfs_assets_file != nullptr) {

				PHYSFS_writeBytes(physfs_assets_file, (const void*)buffer, size);
				PHYSFS_close(physfs_assets_file);

			}


			if (buffer != nullptr) {
				delete[] buffer;
				buffer = nullptr;
			}

		}
		

		fclose(file);
	}

	return size;
}

void ModuleFileSystem::SaveFile(const char* newFilePath, char* buffer, uint size) {

	PHYSFS_file* physfs_lib_file = nullptr;
	physfs_lib_file = PHYSFS_openWrite(newFilePath);

	if (physfs_lib_file != nullptr) {

		PHYSFS_writeBytes(physfs_lib_file, (const void*)buffer, size);
		PHYSFS_close(physfs_lib_file);

	}

}

bool ModuleFileSystem::CleanUp() {

	std::string folder_name = LIB_MESH_PATH;

	vector<string> chadMeshFilesVec = App->imp->GetFilesInFolder(folder_name);

	for (int i = 0; i < chadMeshFilesVec.size(); i++)
	{
		
		std::string fileName_s = chadMeshFilesVec[i].substr(chadMeshFilesVec[i].find_last_of('\\') + 1);

		folder_name = LIB_MESH_PATH;
		folder_name += fileName_s;

		if (remove(folder_name.c_str()) == 0)
		{
			LOG("Chad file removed");
		}
		
	}

	folder_name = LIB_MATERIAL_PATH;

	vector<string> chadMatFilesVec = App->imp->GetFilesInFolder(folder_name);

	for (int i = 0; i < chadMatFilesVec.size(); i++)
	{

		std::string fileName_s = chadMatFilesVec[i].substr(chadMatFilesVec[i].find_last_of('\\') + 1);

		folder_name = LIB_MATERIAL_PATH;
		folder_name += fileName_s;

		if (remove(folder_name.c_str()) == 0)
		{
			LOG("Chad file removed");
		}

	}

	folder_name = LIB_MATERIAL_PATH;

	if (remove(folder_name.c_str()) == 0)
	{
		LOG("Library folder removed");
	}
	

	return true;
}
