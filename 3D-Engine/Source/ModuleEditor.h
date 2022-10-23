#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
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

	void Draw();

	bool DrawMenuBar();

	void AddLogs(const char* text);

	void OpenWeb(const char* url)const;

	void SetOrgName(const char* OrgName);

	void SetAppName(const char*  AppName);
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

	void ConfigAppXXX();


	void ConfigWindowXXX();

	void ConfigHardwareXXX();

	void ConfigInputXXX();

	void ConfigPathXXX();

	void ConfigAudioXXX();

	void CloseEngine();

	void CheckGLCapabilities();


	


public:
	bool close=false;
	bool scroll = false;
	LCG randomLCG;
	
	std::vector<std::string> login;
	
	//ImGuiTextBuffer logs;
private:
	int winWidth;
	int winHeight;
	int selectedRenderMode;

	bool gl_depthTestEnabled;
	bool gl_cullFaceEnabled;
	bool gl_lightingEnabled;
	bool gl_colorMaterialEnabled;
	bool gl_texture2dEnabled;
	bool gl_lineSmoothEnabled;
	bool gl_fogEnabled;

	const char* tdnow,
		*altvec, 
		*AVX,
		*AVX2, 
		*MMX, 
		*RDTSC,
		*SSE,
		*SSE2,
		*SSE3,
		*SSE41,
		*SSE42;
	
};
