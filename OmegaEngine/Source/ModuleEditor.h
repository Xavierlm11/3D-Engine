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
#include "ImGui/imgui.h"

#include <string.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL/include/SDL_opengles2.h>
#else
#include <SDL/include/SDL_opengl.h>
#endif

#include "MathGeoLib/include/Algorithm/Random/LCG.h"

#include "ModuleScene.h"


class ModuleEditor : public Module
{
public:

	enum class InspectorShow
	{
		NONE,
		GAMEOBJECT,
		ASSET,
	};
	
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();


	// Our state
	//windows
	bool show_demo_window = false;
	bool show_another_window = false;
	bool show_console_window = false;
	bool show_render3d_window = false;
	bool show_assets_window = false;
	bool show_config_window = false;
	bool show_about_window = false;
	//shapes
	bool load_house_shape = false;
	bool load_cube_shape = false;
	bool load_sphere_shape = false;
	bool load_pyramid_shape = false;
	bool load_cylinder_shape = false;

	ModuleScene::Shapes selectedShape;


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void Draw();

	void DrawSceneViewport();

	void DrawGameViewport();

	bool DrawMenuBar();

	void AddLogs(const char* text);

	void OpenWeb(const char* url)const;

	void SetOrgName(const char* OrgName);

	void SetAppName(const char*  AppName);
	void CreateSphere(const char* resName, float force);
private:

	//Windows
	void ConsoleWindow();
	void Render3DWindow();
	void AssetsWindow();
	void ConfigWindow();
	void AboutWindow();

	//Bar Options
	void BarFile();
	void BarWindows();
	void BarXXX();

	void LoadShape(const char* resName);
	void BarShapes();

	void BarPlay();
	void BarPause();

	void ConfigAppXXX();


	void ConfigWindowXXX();

	void ConfigHardwareXXX();

	void ConfigInputXXX();

	void ConfigPathXXX();


	void CloseEngine();

	void CheckGLCapabilities();
	;

	void ShowAssetInfo();

	void GOList();

	
	int selection_mask;

	//void SelectChilds();
	void DeleteGo();

	


public:
	int GOIndex;
	int showingGOIndex;
	int assetSelelected;
	InspectorShow inspectorShowing;

	bool close=false;
	bool scroll = false;
	LCG randomLCG;
	bool hasToMoveSelection = true;
	
	std::vector<GameObject*> gameObjectsShowing;
	std::vector<std::string> login;


	int selectedRenderMode;

	bool gl_depthTestEnabled;
	bool gl_cullFaceEnabled;
	bool gl_lightingEnabled;
	bool gl_colorMaterialEnabled;
	bool gl_texture2dEnabled;
	bool gl_lineSmoothEnabled;
	bool gl_fogEnabled;

	//fog
	int selectedFogMode;
	float fog_density;
	float fog_start;
	float fog_end;
	float fog_index;
	float fog_color[4] = {0,0,0,0};
	float fog_coord;

	GameObject* selectedObj = nullptr;

	bool playPressed;
	bool pausePressed;

private:

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

private:

	float AspRatioScene, AspRatioGame;

	GLfloat* cam;
	
};
