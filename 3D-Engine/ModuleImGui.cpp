#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"


ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleImGui::~ModuleImGui()
{

}

// Called before render is available
bool ModuleImGui::Init()
{
	LOG("Init ImGui Module");
	
	bool ret = true;

	return ret;
}

// Called every update
update_status ModuleImGui::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleImGui::CleanUp()
{
	LOG("Quitting ImGui Module");

	return true;
}