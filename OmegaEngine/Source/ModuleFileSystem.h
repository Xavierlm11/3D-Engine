#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();
	
	bool Init();

	uint FileToBuffer(const char* filePath, char** fileBuffer)const;
	uint ImportFileToAssets(const char* fileName);
	
	/*bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint FileToBuffer(const char* file, char** buffer);*/
};
