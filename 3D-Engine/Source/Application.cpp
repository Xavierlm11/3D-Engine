#include "Application.h"
#include <list>

extern Application* External = nullptr;

Application::Application()
{
	External = this;
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules

	AddModule(window);
	
	AddModule(camera);
	AddModule(input);
	
	

	// Scenes
	AddModule(scene);
	
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
	
}

Application::~Application()
{
	/*std::vector<Module>::iterator item ;
	 item = list_modules.begin();*/
	/*while(item!= list_modules->end())
	{
		delete  list_modules[i];
		item++;
	}*/
	for (int i= list_modules.size()-1; i >=0; i--)
	{
		delete  list_modules[i];
		list_modules[i] = nullptr;
	}
	list_modules.clear();
	/*for (std::list<Module>::const_iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete list_mo;
	}
	foo_list.clear();
	for (auto& it : list_modules)delete it; */
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	/*std::list<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		
		ret = item->data->Init();
		item = item->next;
	}*/
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		  list_modules[i]->Init();
		
	}
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	/*item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}*/
	for (int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret=list_modules[i]->Start();

	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
	//fps[0]->Start()
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	PrepareUpdate();

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate(dt);

	}

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update(dt);

	}

	for (int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate(dt);

	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	/*p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}*/

	for (int i = 0; i < list_modules.size() && ret == true; i--)
	{
		ret = list_modules[i]->CleanUp();

	}
	External = nullptr;
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

