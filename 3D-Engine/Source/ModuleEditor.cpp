#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include "SDL/include/SDL.h"
//#include <shellapi.h>
#include "ModuleRenderer3D.h"

#include "MathGeoLib/include/Algorithm/Random/LCG.h"


#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL/include/SDL_opengles2.h>
#else
#include <SDL/include/SDL_opengl.h>
#endif

#include <string>
#include <xstring>


#ifdef _DEBUG
#pragma comment( lib, "Source/External/MathGeoLib/libx86/MGLDebug/MathGeoLib.lib")
#else
#pragma comment (lib, "Source/External/MathGeoLib/libx86/MGLRelease/MathGeoLib.lib")
#endif

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	selectedRenderMode = 0;
	gl_depthTestEnabled = true;
	gl_cullFaceEnabled = true;
	gl_lightingEnabled = true;
	gl_colorMaterialEnabled = true;
	gl_texture2dEnabled = true;
	gl_lineSmoothEnabled = false;
	gl_fogEnabled = false;

}

// Destructor
ModuleEditor::~ModuleEditor()
{
	//logs.clear();
	login.clear();

}

// Called before render is available
bool ModuleEditor::Init()
{
	LOG("Init ImGui Module");

	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);

	return ret;
}

// Called every update
update_status ModuleEditor::Update(float dt)
{
	// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	//SDL_Event event;
	/*while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			done = true;
	}*/

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   /* if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);*/

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		//static float f = 0.0f;
		//static int counter = 0;

		//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//    counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();
	}

	//// 3. Show another simple window.
	//if (show_another_window)
	//{
	//    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//    ImGui::Text("Hello from another window!");
	//    if (ImGui::Button("Close Me"))
	//        show_another_window = false;
	//    ImGui::End();
	//}

	if (gl_depthTestEnabled == true) {
		if (glIsEnabled(GL_DEPTH_TEST) == false) {
			glEnable(GL_DEPTH_TEST);
		}
	}
	else {
		if (glIsEnabled(GL_DEPTH_TEST) == true) {
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (gl_cullFaceEnabled == true) {
		if (glIsEnabled(GL_CULL_FACE) == false) {
			glEnable(GL_CULL_FACE);
		}
	}
	else {
		if (glIsEnabled(GL_CULL_FACE) == true) {
			glDisable(GL_CULL_FACE);
		}
	}

	if (gl_lightingEnabled == true) {
		if (glIsEnabled(GL_LIGHTING) == false) {
			glEnable(GL_LIGHTING);
		}
	}
	else {
		if (glIsEnabled(GL_LIGHTING) == true) {
			glDisable(GL_LIGHTING);
		}
	}

	if (gl_colorMaterialEnabled == true) {
		if (glIsEnabled(GL_COLOR_MATERIAL) == false) {
			glEnable(GL_COLOR_MATERIAL);
		}
	}
	else {
		if (glIsEnabled(GL_COLOR_MATERIAL) == true) {
			glDisable(GL_COLOR_MATERIAL);
		}
	}

	if (gl_texture2dEnabled == true) {
		if (glIsEnabled(GL_TEXTURE_2D) == false) {
			glEnable(GL_TEXTURE_2D);
		}
	}
	else {
		if (glIsEnabled(GL_TEXTURE_2D) == true) {
			glDisable(GL_TEXTURE_2D);
		}
	}

	if (gl_lineSmoothEnabled == true) {
		if (glIsEnabled(GL_LINE_SMOOTH) == false) {
			glEnable(GL_LINE_SMOOTH);
		}
	}
	else {
		if (glIsEnabled(GL_LINE_SMOOTH) == true) {
			glDisable(GL_LINE_SMOOTH);
		}
	}


	if (gl_fogEnabled == true) {
		if (glIsEnabled(GL_FOG) == false) {
			glEnable(GL_FOG);
		}
	}
	else {
		if (glIsEnabled(GL_FOG) == true) {
			glDisable(GL_FOG);
		}
	}




	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Quitting ImGui Module");
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	login.clear();
	//logs.clear();

	return true;
}

bool ModuleEditor::DrawMenuBar()
{
	//LOG("MenuBAR");
  // ImGui::Begin("Menu bar", NULL, ImGuiWindowFlags_MenuBar);
	/*if(ImGui::BeginMenuBar() )
	{

	}*/
	if (ImGui::BeginMainMenuBar())
	{
		BarFile();
		BarWindows();
		

		ImGui::EndMainMenuBar();
	}

	return true;
}
void ModuleEditor::AddLogs(const char* text)
{
	// logs.appendf(text);

	login.push_back(text);
	scroll = true;

}

void ModuleEditor::OpenWeb(const char* url)const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);

}

void ModuleEditor::ConsoleWindow()
{
	std::string* e = nullptr;

	if (ImGui::Begin("Console", &show_console_window)) {

		// ImGui::Text(logs.begin());
		for (unsigned int i = 0; i < login.size(); ++i) {
			e = &login[i];

			ImGui::Text(e->c_str());
		}
		if (scroll) ImGui::SetScrollHereY(1.0f), scroll = false;
	}
	
	//LOG("%i", a);
	//a++;

	ImGui::End();
}

void ModuleEditor::Render3DWindow() {

	ImGui::Begin("Scene View", &show_render3d_window);
	
	//// Using the _simplified_ one-liner ListBox() api here
	//// See "List boxes" section for examples of how to use the more flexible BeginListBox()/EndListBox() api.
	//const char* items[] = { "Normal", "Wireframe" };
	//static int item_current = 1;
	//ImGui::ListBox("View Mode", &item_current, items, IM_ARRAYSIZE(items));
	////ImGui::SameLine(); HelpMarker(
	////	"Using the simplified one-liner ListBox API here.\nRefer to the \"List boxes\" section below for an explanation of how to use the more flexible and general BeginListBox/EndListBox API.");

	//const char* viewModeItems[] = { "Normal", "Wireframe" };
	//ImGui::ListBox("View Mode",0,viewModeItems, 2);

		const char* modes[] = { "Normal", "Wireframe"};

		if (ImGui::Button("Select..."))
			ImGui::OpenPopup("Render mode");
		ImGui::SameLine();
		ImGui::TextUnformatted(selectedRenderMode == -1 ? "<None>" : modes[selectedRenderMode]);
		if (ImGui::BeginPopup("Render mode"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(modes); i++) {
				if (ImGui::Selectable(modes[i])) {
					selectedRenderMode = i;
					App->renderer3D->mode = (ModuleRenderer3D::RenderMode)selectedRenderMode;
					
				}
			}
				
			ImGui::EndPopup();
		}

		if (ImGui::TreeNode("Render Options"))
		{
			ImGui::Checkbox("Depth Test", &gl_depthTestEnabled);
			ImGui::Checkbox("Cull Face", &gl_cullFaceEnabled);
			ImGui::Checkbox("Lighting", &gl_lightingEnabled);
			ImGui::Checkbox("Color Material", &gl_colorMaterialEnabled);
			ImGui::Checkbox("Texture 2D", &gl_texture2dEnabled);
			ImGui::Checkbox("Line Smooth", &gl_lineSmoothEnabled);
			ImGui::Checkbox("Fog", &gl_fogEnabled);
			

			ImGui::TreePop();
		}



	/*int renderX = ImGui::GetWindowPos().x;
	int renderY = ImGui::GetWindowPos().y;
	int renderW = ImGui::GetWindowWidth();
	int renderH = ImGui::GetWindowHeight();*/
	//App->renderer3D->OnResize(renderX, -renderY + renderH, renderW, renderH);

	//SDL_GetWindowSize(App->window->window, &winWidth, &winHeight);
	//App->renderer3D->OnResize(0, 0, winWidth, winHeight);

	ImGui::End();
}

void ModuleEditor::ConfigWindow() {

	BarXXX();
}

void ModuleEditor::AboutWindow() {

	if (ImGui::Begin("About", &show_about_window))
	{
		static char AppName[30];
		strcpy_s(AppName,30,App->EngName.c_str());
		ImGui::Text("App name:");
		ImGui::SameLine();
		ImGui::TextColored({ 0,255,232,1 }, "%s", App->EngName.c_str());
		ImGui::Text("");

		ImGui::Text("By:");
		if (ImGui::Button("Xavier Casado Benitez"))
		{
			OpenWeb("https://github.com/Akage369");
		}
		if (ImGui::Button("Xavier Lopez Martin"))
		{
			OpenWeb("https://github.com/Xavierlm11");
		}
		ImGui::Text("");

		ImGui::TextWrapped("This engine will be the most revolutionary engine since the invention of the kebab");
		ImGui::Text("");

		if (ImGui::Button("Github Repository")) {

			for (unsigned int i = 0; i <= 10; ++i)OpenWeb("https://www.raylib.com/");
			OpenWeb("https://youtu.be/SCpV7OmmR60");
		}	
		ImGui::Text("");

		ImGui::Text("3rd Party Libreries Used:");
		ImGui::Text("");
		//ImGui::ColorButton();
		ImGui::Text("License:");
		ImGui::SameLine();
		ImGui::TextColored({ 0,255,232,1 }, "MIT License");
		ImGui::Text("");

		ImGui::Text("Copyright (c) 2022 Xavierlm11");
		ImGui::Text("");
		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copyof this software and associated documentation files(the 'Software'), to dealin the Software without restriction, including without limitation the rightsto use, copy, modify, merge, publish, distribute, sublicense, and /or sellcopies of the Software, and to permit persons to whom the Software isfurnished to do so, subject to the following conditions : ");
		ImGui::Text("");
		ImGui::TextWrapped({ "The above copyright notice and this permission notice shall be included in all"
			"copies or substantial portions of the Software." });
		ImGui::Text("");
		ImGui::TextWrapped({"THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
			"SOFTWARE."});
	}
	ImGui::End();
}

void ModuleEditor::Draw() {

	DrawMenuBar();

	if (show_console_window) {

		ConsoleWindow();
	}

	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_render3d_window) {

		Render3DWindow();
	}

	if (show_config_window) {

		ConfigWindow();
	}

	if (show_about_window) {

		AboutWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::BarFile() {
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{

		}
		if (ImGui::MenuItem("Save"))
		{

			App->scene->SaveScene();
		}
		if (ImGui::MenuItem("Load"))
		{

			App->scene->LoadScene();
		}
		if (ImGui::MenuItem("Close"))
		{
			close = true;

		}
		ImGui::EndMenu();
	}
}

void ModuleEditor::BarWindows() {
	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::Checkbox("Console Window", &show_console_window))
		{

		}
		if (ImGui::Checkbox("Demo Window", &show_demo_window))
		{

		}
		if (ImGui::Checkbox("Scene View Window", &show_render3d_window))
		{

		}
		if (ImGui::Checkbox("Configuration", &show_config_window))
		{

		}
		if (ImGui::Checkbox("About", &show_about_window))
		{

		}


		ImGui::EndMenu();
	}
	
}
void ModuleEditor::BarXXX() {
	if (ImGui::Begin("Configuration", &show_config_window)) {
		ImGui::Text("Options");
		if (ImGui::CollapsingHeader("APP"))
		{
			ConfigAppXXX();
		}
		if (ImGui::CollapsingHeader("Window"))
		{

			ConfigWindowXXX();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ConfigHardwareXXX();
		}



	}

	ImGui::End();
}

void ModuleEditor::ConfigAppXXX()
{
	static char AppName[30];
	strcpy_s(AppName, 30, App->EngName.c_str());

	static char OrgName[60];
	strcpy_s(OrgName, 60, App->OrgName.c_str());

	ImGui::Text("App name:");
	ImGui::SameLine();
	if (ImGui::InputText("", AppName, 30, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{

		if (AppName != nullptr)
		{
			SDL_SetWindowTitle(App->window->window, AppName);
		}
	}
	ImGui::Text("Organization:");
	ImGui::SameLine();
	if (ImGui::InputText("", OrgName, 60, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{

		if (OrgName != nullptr)
		{
			SDL_SetWindowTitle(App->window->window, OrgName);
		}
	}
	//ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
}

void ModuleEditor::ConfigWindowXXX()
{
	ImGui::Text("Brightness:");
	ImGui::SameLine();
	float brigth = SDL_GetWindowBrightness(App->window->window);
	if (ImGui::SliderFloat("Brightness", &brigth, 0, 2))
	{
		SDL_SetWindowBrightness(App->window->window, brigth);
	}

	ImGui::Text("Width:");
	ImGui::SameLine();
	int width = App->window->winWidth;
	if (ImGui::SliderInt("Width", (int*)&width, MIN_WIDTH, MAX_WIDTH))
	{
		App->window->winWidth = width;
		SDL_SetWindowSize(App->window->window, width, App->window->winHeight);

		SDL_SetWindowBrightness(App->window->window, brigth);
	}
	ImGui::Text("Height:");
	ImGui::SameLine();
	int height = App->window->winHeight;
	if (ImGui::SliderInt("Height", (int*)&height, MIN_HEIGHT, MAX_HEIGHT))
	{
		App->window->winHeight = height;
		SDL_SetWindowSize(App->window->window, App->window->winWidth, height);
		SDL_SetWindowBrightness(App->window->window, brigth);
}
	if (ImGui::Button("Reset"))
	{
		SDL_SetWindowSize(App->window->window, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_SetWindowBrightness(App->window->window, 1);

	}
}

void ModuleEditor::ConfigHardwareXXX()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
	const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
														// const GLubyte* vram = glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX); // Returns a hint to the model

														//const GLubyte* version = glGetString(GL_VERSION); // Returns a hint to the model

	ImGui::Text("SDL Version: ");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%u.%u.%u", compiled.major, compiled.minor, compiled.patch);
	ImGui::Separator();//--------------
	ImGui::Text("CPUs:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%d (cache %i Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%f Gb", (float)SDL_GetSystemRAM());
	ImGui::Text("Caps:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%s %s", SDL_Has3DNow() ? "3DNow" : "", SDL_HasAltiVec() ? "AltiVec" : "");
	ImGui::Separator();//--------------
	ImGui::Text("GPU:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "vendor %s device %s", vendor);
	ImGui::Text("Brand:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%s", renderer);
	ImGui::Text("VRAM Budget:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%i", 1);
	ImGui::Text("VRAM Usage:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
	ImGui::Text("VRAM Avaliable:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
	ImGui::Text("VRAM Reserved:");
	ImGui::SameLine();
	ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
}

