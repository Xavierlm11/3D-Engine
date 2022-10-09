#pragma once

#include <iostream>
//#include <vector>
#include <list>


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"	
#include "ModuleEditor.h"
#include "ModuleScene.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;

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
	//void logs(const char* text);
private:

	void AddModule(Module* mod);
	
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* External;