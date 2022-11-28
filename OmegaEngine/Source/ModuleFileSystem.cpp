#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#include "PhysFS/include/physfs.h"
#include <iostream>
#include "SDL/include/SDL_filesystem.h"

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
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
		LOG("Error adding a path: %s\n", PHYSFS_getLastError());
	}

	return true;
}


uint ModuleFileSystem::FileToBuffer(const char* filePath, char** fileBuffer)const {

	uint ret = 0;


	/*std::string filedir_s = filePath;
	std::string fileName_s = filedir_s.substr(filedir_s.find_last_of('\\') + 1);
	std::string extension_s = fileName_s.substr(fileName_s.find_last_of('.') + 1);

	std::string assetsPath_s = "Assets/";
	const char* fileName = fileName_s.c_str();
	std::string finalPath = assetsPath_s + fileName;*/

	PHYSFS_file* pysfsFile = PHYSFS_openRead(filePath);

	if (pysfsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(pysfsFile);

		if (size > 0)
		{
			*fileBuffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(pysfsFile, *fileBuffer, 1, size);

			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", filePath, PHYSFS_getLastError());
				if (fileBuffer != nullptr) {
					delete[] fileBuffer;
					fileBuffer = nullptr;
				}
			}
			else
			{
				ret = readed;
				(*fileBuffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(pysfsFile) == 0) {
			LOG("File System error while closing file %s: %s\n", filePath, PHYSFS_getLastError());
		}
	}
	else {
		LOG("File System error while opening file %s: %s\n", filePath, PHYSFS_getLastError());
	}


	return ret;
}

uint ModuleFileSystem::ImportFileToAssets(const char* fileName) {

	uint size = 0;

	std::string dir = "Assets/";
	

	std::FILE* file;
	fopen_s(&file, fileName, "rb");

	if (file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		char* buffer = new char[size];

		size = fread(buffer, 1, size, file);
		if (size > 0)
		{
			std::string dropped_filedir_s = fileName;
			std::string fileName_s = dropped_filedir_s.substr(dropped_filedir_s.find_last_of('\\') + 1);
			std::string totalDir = dir + fileName_s;
			
			PHYSFS_file* physfs_file = nullptr;
			physfs_file  = PHYSFS_openWrite(totalDir.c_str());

			if (physfs_file != nullptr)
			{
				int work = PHYSFS_writeBytes(physfs_file, (const void*)buffer, size);
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


