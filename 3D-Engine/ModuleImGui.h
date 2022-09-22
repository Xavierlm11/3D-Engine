#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleImGui : public Module
{
public:

	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

};
