#pragma once
#include "Module.h"
#include "Globals.h"
#include "Mesh.h"


class ModuleImport : public Module
{
public:
	ModuleImport(Application* app, bool start_enabled = true);
	~ModuleImport();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void LoadFile(const char* path);

	VertexDatas a;
};
