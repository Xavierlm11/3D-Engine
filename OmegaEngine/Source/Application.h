#pragma once

#include <iostream>
//#include <vector>
#include <list>
#include <string>

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleImport.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"	
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "ModulePhysics.h"


class Application
{
public:
	ModuleImport* imp;
	ModuleWindow* window;
	ModuleInput* input;
	
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;
	ModuleFileSystem* fileSystem;
	ModuleResources* resources;
	ModulePhysics3D* physics;

private:

	Timer	ms_timer;
	float	dt;
	Timer lastSecFrameTime;
	std::vector<Module*> list_modules;
	std::vector<Timer*> fps;
	
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	std::string EngName=TITLE;//ENGINE NAME
	std::string OrgName = ORGANIZATION;//ENGINE NAME
	//void logs(const char* text);
	void SetName(const char* name);
	
	

	LCG rand;

private:

	void AddModule(Module* mod);

	
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* External;