#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#include "PhysFS/include/physfs.h"
#include <iostream>
#include "SDL/include/SDL_filesystem.h"

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

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0) {
		LOG("Error creating write directory: %s\n", PHYSFS_getLastError());

	}
		
	if (PHYSFS_mount(".", nullptr, 1) == 0)
	{
		LOG("Error adding a path: %s\n", PHYSFS_getLastError());
	}

	if (PHYSFS_mount("Assets", nullptr, 1) == 0)
	{
		LOG("Error adding the path: %s\n", PHYSFS_getLastError());
	}

	if (PHYSFS_mount("Library", nullptr, 1) == 0)
	{
		LOG("Error adding the path: %s\n", PHYSFS_getLastError());
	}

	if (PHYSFS_mount("Library/Meshes", nullptr, 1) == 0)
	{
		LOG("Error adding the path: %s\n", PHYSFS_getLastError());
	}

	if (PHYSFS_mount("Library/Materials", nullptr, 1) == 0)
	{
		LOG("Error adding the path: %s\n", PHYSFS_getLastError());
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
	//std::string libDir = "Library/";
	

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

