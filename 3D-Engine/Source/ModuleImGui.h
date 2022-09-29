#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include "SDL/include/SDL.h"

#include <string.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL/include/SDL_opengles2.h>
#else
#include <SDL/include/SDL_opengl.h>
#endif


//#ifdef _DEBUG
//#pragma comment( lib, "Source/External/MathGeoLib/libx86/Debug/MathGeoLib.lib")
//#else
//#pragma comment (lib, "Source/External/MathGeoLib/libx86/Release/MathGeoLib.lib")
//#endif



#include "MathGeoLib/include/Algorithm/Random/LCG.h"

#pragma comment( lib, "Source/External/MathGeoLib/libx86/Debug/MathGeoLib.lib")


class ModuleImGui : public Module
{
public:

	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//ImGuiIO io;
	SDL_GLContext gl_context;


	bool MenuBar();

	bool close=true;

	//LCG randomLCG;


};
