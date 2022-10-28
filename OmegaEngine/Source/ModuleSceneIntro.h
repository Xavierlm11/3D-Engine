#pragma once
#include "Module.h"

#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

};
