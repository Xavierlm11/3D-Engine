#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include "SDL/include/SDL.h"
#include <vector>

#include <string.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL/include/SDL_opengles2.h>
#else
#include <SDL/include/SDL_opengl.h>
#endif

#include "MathGeoLib/include/Algorithm/Random/LCG.h"




class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();


	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	bool show_console_window = false;
	bool show_render3d_window = false;
	bool show_config_window = false;
	bool show_about_window = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//ImGuiIO io;
	SDL_GLContext gl_context;

	void Draw();

	bool DrawMenuBar();

	void AddLogs(const char* text);

	void OpenWeb(const char* url)const;

private:

	//Windows
	void ConsoleWindow();
	void Render3DWindow();
	void ConfigWindow();
	void AboutWindow();

	//Bar Options
	void BarFile();
	void BarWindows();
	void BarXXX();

	


public:
	bool close=false;
	bool scroll = false;
	LCG randomLCG;
	int a=0;
	std::vector<std::string> login;
	
	//ImGuiTextBuffer logs;
private:
	int winWidth;
	int winHeight;
};
