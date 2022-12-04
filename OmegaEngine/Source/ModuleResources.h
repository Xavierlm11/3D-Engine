#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleResources : public Module
{
public:

	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();
};
