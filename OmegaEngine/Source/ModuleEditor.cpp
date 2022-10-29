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
	ModuleScene::Shapes::NONE;


	tdnow =  " ";
	
	altvec = " ";
	AVX =  " ";
	AVX2 =  " ";
	MMX =  " ";
	RDTSC =  " ";
	SSE =  " ";
	SSE2 =  " ";
	SSE3 =  " ";
	SSE41 =  " ";
	SSE42 =  " ";

}

// Destructor
ModuleEditor::~ModuleEditor()
{
	
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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	// Setup Dear ImGui style

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);

	tdnow = SDL_Has3DNow() ? "3DNow," : " ";
	//tdnow =  "3DNow,";
	altvec = SDL_HasAltiVec() ? "AltiVec," : " ";
	AVX = SDL_HasAVX() ? "AVX," : " ";
	AVX2 = SDL_HasAVX2() ? "AVX2," : " ";
	MMX = SDL_HasMMX() ? "MMX," : " ";
	RDTSC = SDL_HasRDTSC() ? "RDTSC," : " ";
	SSE = SDL_HasSSE() ? "SSE," : " ";
	SSE2 = SDL_HasSSE2()? "SSE2," : " ";
	SSE3 = SDL_HasSSE3() ? "SSE3," : " ";
	SSE41 = SDL_HasSSE41() ? "SSE41," : " ";
	SSE42 = SDL_HasSSE42() ? "SSE42," : " ";

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(main_viewport->WorkPos);
	ImGui::SetNextWindowSize(main_viewport->Size);
	ImGui::SetNextWindowViewport(main_viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dock", nullptr, flags);

	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("Dock");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();

	return UPDATE_CONTINUE;
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
	

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

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
		//
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

	CheckGLCapabilities();
	CheckShapes();

	if (App->input->CallClose)
	{
		CloseEngine();
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

	selectedShape = ModuleScene::Shapes::NONE;

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
		BarShapes();
		

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
		
		ImGui::Text("App name:");
		ImGui::SameLine();
		ImGui::TextColored({ 0,255,232,1 }, "%s", App->EngName.c_str());
		ImGui::Text("Organization name:");
		ImGui::SameLine();
		ImGui::TextColored({ 0,255,232,1 }, "%s", App->OrgName.c_str());

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

			//for (unsigned int i = 0; i <= 10; ++i)OpenWeb("https://www.raylib.com/");
			OpenWeb("https://youtu.be/OgZzUJud3Q4");
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

void ModuleEditor::BarShapes() {
	if (ImGui::BeginMenu("Shapes"))
	{
		if (ImGui::Checkbox("House", &load_house_shape))
		{

		}
		if (ImGui::Checkbox("Cube", &load_cube_shape))
		{

		}
		if (ImGui::Checkbox("Sphere", &load_sphere_shape))
		{

		}
		if (ImGui::Checkbox("Pyramid", &load_pyramid_shape))
		{

		}
		if (ImGui::Checkbox("Cylinder", &load_cylinder_shape))
		{

		}


		ImGui::EndMenu();
	}

}

void ModuleEditor::BarXXX() {
	if (ImGui::Begin("Configuration", &show_config_window)) {
		ImGui::Text("Options");
		
			ConfigAppXXX();
		
			ConfigWindowXXX();
		
			ConfigHardwareXXX();

			ConfigInputXXX();

			ConfigPathXXX();

			ConfigAudioXXX();
		}


	ImGui::End();

}

void ModuleEditor::ConfigAppXXX()
{
	if (ImGui::CollapsingHeader("APP"))
	{
		static char AppName[150];
		strcpy_s(AppName, 150, App->EngName.c_str());

		ImGui::Text("App name:");
		ImGui::SameLine();
		if (ImGui::InputText(" ", AppName, 150, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			SetAppName(AppName);
		}

		static char OrgName[150];
		strcpy_s(OrgName, 150, App->OrgName.c_str());
		ImGui::Text("Organization:");
		ImGui::SameLine();
		if (ImGui::InputText("  ", OrgName, 150, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			SetOrgName(OrgName);
		}

		ImGui::Text("FPS:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%.1f", ImGui::GetIO().Framerate);

		ImGui::Text("MS:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%.3f", 1000 / ImGui::GetIO().Framerate);
	}
}

void ModuleEditor::SetOrgName(const char*  OrgName)
{
	if (OrgName != nullptr)
	{
		App->OrgName = OrgName;
		
	}
}

void ModuleEditor::SetAppName(const char*  AppName)
{
	if (AppName != nullptr)
	{
		App->EngName = AppName;
		SDL_SetWindowTitle(App->window->window, AppName);
	}
}

void ModuleEditor::ConfigWindowXXX()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Brightness:");
		ImGui::SameLine();
		float brigth = SDL_GetWindowBrightness(App->window->window);
		if (ImGui::SliderFloat(" ", &brigth, 0, 2))
		{
			SDL_SetWindowBrightness(App->window->window, brigth);
		}

		ImGui::Text("Width:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%ipx", App->window->winWidth);
		/*int width = App->window->winWidth;
		if (ImGui::SliderInt("Width", (int*)&width, MIN_WIDTH, MAX_WIDTH))
		{
			App->window->winWidth = width;
			SDL_SetWindowSize(App->window->window, width, App->window->winHeight);

		
		}*/
		ImGui::Text("Height:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%ipx", App->window->winHeight);
		/*int height = App->window->winHeight;
		if (ImGui::SliderInt("Height", (int*)&height, MIN_HEIGHT, MAX_HEIGHT))
		{
			App->window->winHeight = height;
			SDL_SetWindowSize(App->window->window, App->window->winWidth, height);
			
		}*/
		if (ImGui::Button("Reset"))
		{
			SDL_SetWindowSize(App->window->window, SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_SetWindowBrightness(App->window->window, 1);

		}
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

	if (ImGui::CollapsingHeader("Hardware"))
	{
		

		ImGui::Text("SDL Version: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%u.%u.%u", compiled.major, compiled.minor, compiled.patch);
		ImGui::Separator();//--------------
		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%d (cache %i Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%.1f Gb", (float)SDL_GetSystemRAM() / 1024.f);
		ImGui::Text("Caps:");
		ImGui::SameLine();
		//ImGui::PushStyleColor(ImGuiCol_Text, { 255,0,0,1 });
		ImGui::TextWrapped(/*{ 255,0,0,1 },*/ "%s%s%s%s%s%s%s%s%s%s%s", tdnow, altvec, AVX, AVX2, MMX, RDTSC, SSE, SSE2, SSE3, SSE41, SSE42);
		/*ImGui::SameLine();
		ImGui::Text("%s%s%s%s%s%s",  RDTSC, SSE, SSE2, SSE3,SSE41, SSE42);*/
		ImGui::Separator();//--------------
		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "vendor %s device %s", vendor);
		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%s", renderer);
		/*ImGui::Text("VRAM Budget:");
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
		ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);*/
	}
}

void ModuleEditor::ConfigInputXXX()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "X: %i Y:%i", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Text("Wheel Motion:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%i", App->input->GetMouseZ());

	}
}

void ModuleEditor::ConfigPathXXX()
{
	if (ImGui::CollapsingHeader("Path"))
	{
		ImGui::Text("Base path:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%s", SDL_GetBasePath() );
	}
}

void ModuleEditor::ConfigAudioXXX()
{
	if (ImGui::CollapsingHeader("Audio"))
	{
		ImGui::Text("Work in progress");
	}
}

void ModuleEditor::CloseEngine()
{
	if (ImGui::Begin("Exit"))
	{
		ImGui::TextWrapped("Are you sure you want to partirle piernas to Albert");
		if (ImGui::Button("YES"))
		{
			/*OpenWeb("https://images.cdn.circlesix.co/image/1/700/0/uploads/posts/2018/09/63125b0ed58a7f29b69b20bb066fd184.jpg");
			OpenWeb("https://youtu.be/OgZzUJud3Q4");*/
			App->input->close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("NO"))
		{
			App->input->CallClose = false;
		}

	}
	ImGui::End();
}

void ModuleEditor::CheckShapes() {

	if (load_house_shape == true) {

		

		if (App->scene->house_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::HOUSE) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshes);
			}
			App->scene->LoadHouse(&App->scene->models[0]->meshes);
			for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else 
	{
		if (App->scene->house_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshes);
		}
	}

	if (load_cube_shape == true) {

		

		if (App->scene->cube_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::CUBE) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshes);
			}
			App->scene->LoadCube(&App->scene->models[0]->meshes);
			for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
			{
				App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->cube_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshes);
		}
	}

	if (load_sphere_shape == true) {

		

		if (App->scene->sphere_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::SPHERE) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshes);
			}
			App->scene->LoadSphere(&App->scene->models[0]->meshes);
			for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
			{
				App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
			
	}
	else
	{
		if (App->scene->sphere_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshes);
		}
	}

	if (load_pyramid_shape == true) {

		
		if (App->scene->pyramid_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::PYRAMID) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshes);
			}

			App->scene->LoadPyramid(&App->scene->models[0]->meshes);
			for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
			{
				App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->pyramid_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshes);
		}
	}

	if (load_cylinder_shape == true) {

	

		if (App->scene->cylinder_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::CYLINDER) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshes);
			}
			App->scene->LoadCylinder(&App->scene->models[0]->meshes);
			for (int i = 0; i < App->scene->models[0]->meshes.size(); i++)
			{
				App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->cylinder_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshes);
		}
	}
	
	if (load_house_shape == false
		&& load_cube_shape == false
		&& load_sphere_shape == false
		&& load_pyramid_shape == false
		&& load_cylinder_shape == false) {

		selectedShape = ModuleScene::Shapes::NONE;
		App->scene->house_loaded = false;
		App->scene->cube_loaded = false;
		App->scene->sphere_loaded = false;
		App->scene->pyramid_loaded = false;
		App->scene->cylinder_loaded = false;
	}

	switch (selectedShape) {
		case  ModuleScene::Shapes::NONE:

			if (load_house_shape == true) {
				load_house_shape = false;
				if (App->scene->house_loaded == true) {
					App->scene->house_loaded = false;
				}
			}
		
			if (load_cube_shape == true) {
				load_cube_shape = false;
				if (App->scene->cube_loaded == true) {
					App->scene->cube_loaded = false;
				}
			}

			if (load_sphere_shape == true) {
				load_sphere_shape = false;
				if (App->scene->sphere_loaded == true) {
					App->scene->sphere_loaded = false;
				}
			}

			if (load_pyramid_shape == true) {
				load_pyramid_shape = false;
				if (App->scene->pyramid_loaded == true) {
					App->scene->pyramid_loaded = false;
				}
			}

			if (load_cylinder_shape == true) {
				load_cylinder_shape = false;
				if (App->scene->cylinder_loaded == true) {
					App->scene->cylinder_loaded = false;
				}
			}

			break;
		
		case  ModuleScene::Shapes::HOUSE:

			if (load_cube_shape == true) {
				load_cube_shape = false;
				if (App->scene->cube_loaded == true) {
					App->scene->cube_loaded = false;
				}
			}

			if (load_sphere_shape == true) {
				load_sphere_shape = false;
				if (App->scene->sphere_loaded == true) {
					App->scene->sphere_loaded = false;
				}
			}

			if (load_pyramid_shape == true) {
				load_pyramid_shape = false;
				if (App->scene->pyramid_loaded == true) {
					App->scene->pyramid_loaded = false;
				}
			}

			if (load_cylinder_shape == true) {
				load_cylinder_shape = false;
				if (App->scene->cylinder_loaded == true) {
					App->scene->cylinder_loaded = false;
				}
			}

			break;
		
		case  ModuleScene::Shapes::CUBE:

			if (load_house_shape == true) {
				load_house_shape = false;
				if (App->scene->house_loaded == true) {
					App->scene->house_loaded = false;
				}
			}

			if (load_sphere_shape == true) {
				load_sphere_shape = false;
				if (App->scene->sphere_loaded == true) {
					App->scene->sphere_loaded = false;
				}
			}

			if (load_pyramid_shape == true) {
				load_pyramid_shape = false;
				if (App->scene->pyramid_loaded == true) {
					App->scene->pyramid_loaded = false;
				}
			}

			if (load_cylinder_shape == true) {
				load_cylinder_shape = false;
				if (App->scene->cylinder_loaded == true) {
					App->scene->cylinder_loaded = false;
				}
			}

			break;

		case  ModuleScene::Shapes::SPHERE:

			if (load_house_shape == true) {
				load_house_shape = false;
				if (App->scene->house_loaded == true) {
					App->scene->house_loaded = false;
				}
			}

			if (load_cube_shape == true) {
				load_cube_shape = false;
				if (App->scene->cube_loaded == true) {
					App->scene->cube_loaded = false;
				}
			}

			if (load_pyramid_shape == true) {
				load_pyramid_shape = false;
				if (App->scene->pyramid_loaded == true) {
					App->scene->pyramid_loaded = false;
				}
			}

			if (load_cylinder_shape == true) {
				load_cylinder_shape = false;
				if (App->scene->cylinder_loaded == true) {
					App->scene->cylinder_loaded = false;
				}
			}

			break;

		case  ModuleScene::Shapes::PYRAMID:

			if (load_house_shape == true) {
				load_house_shape = false;
				if (App->scene->house_loaded == true) {
					App->scene->house_loaded = false;
				}
			}

			if (load_cube_shape == true) {
				load_cube_shape = false;
				if (App->scene->cube_loaded == true) {
					App->scene->cube_loaded = false;
				}
			}

			if (load_sphere_shape == true) {
				load_sphere_shape = false;
				if (App->scene->sphere_loaded == true) {
					App->scene->sphere_loaded = false;
				}
			}

			if (load_cylinder_shape == true) {
				load_cylinder_shape = false;
				if (App->scene->cylinder_loaded == true) {
					App->scene->cylinder_loaded = false;
				}
			}

			break;

		case  ModuleScene::Shapes::CYLINDER:

			if (load_house_shape == true) {
				load_house_shape = false;
				if (App->scene->house_loaded == true) {
					App->scene->house_loaded = false;
				}
			}

			if (load_cube_shape == true) {
				load_cube_shape = false;
				if (App->scene->cube_loaded == true) {
					App->scene->cube_loaded = false;
				}
			}

			if (load_sphere_shape == true) {
				load_sphere_shape = false;
				if (App->scene->sphere_loaded == true) {
					App->scene->sphere_loaded = false;
				}
			}

			if (load_pyramid_shape == true) {
				load_pyramid_shape = false;
				if (App->scene->pyramid_loaded == true) {
					App->scene->pyramid_loaded = false;
				}
			}
			break;
	}
}

void ModuleEditor::CheckGLCapabilities() {
	if (gl_depthTestEnabled == true) {
		if (!glIsEnabled(GL_DEPTH_TEST)) {
			glEnable(GL_DEPTH_TEST);
		}
	}
	else {
		if (glIsEnabled(GL_DEPTH_TEST)) {
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (gl_cullFaceEnabled == true) {
		if (!glIsEnabled(GL_CULL_FACE)) {
			glEnable(GL_CULL_FACE);
		}
	}
	else {
		if (glIsEnabled(GL_CULL_FACE)) {
			glDisable(GL_CULL_FACE);
		}
	}

	if (gl_lightingEnabled == true) {
		if (!glIsEnabled(GL_LIGHTING)) {
			glEnable(GL_LIGHTING);
		}
	}
	else {
		if (glIsEnabled(GL_LIGHTING)) {
			glDisable(GL_LIGHTING);
		}
	}

	if (gl_colorMaterialEnabled == true) {
		if (!glIsEnabled(GL_COLOR_MATERIAL)) {
			glEnable(GL_COLOR_MATERIAL);
		}
	}
	else {
		if (glIsEnabled(GL_COLOR_MATERIAL)) {
			glDisable(GL_COLOR_MATERIAL);
		}
	}

	if (gl_texture2dEnabled == true) {
		if (!glIsEnabled(GL_TEXTURE_2D)) {
			glEnable(GL_TEXTURE_2D);
		}
	}
	else {
		if (glIsEnabled(GL_TEXTURE_2D)) {
			glDisable(GL_TEXTURE_2D);
		}
	}

	if (gl_lineSmoothEnabled == true) {
		if (!glIsEnabled(GL_LINE_SMOOTH)) {
			glEnable(GL_LINE_SMOOTH);
		}
	}
	else {
		if (glIsEnabled(GL_LINE_SMOOTH)) {
			glDisable(GL_LINE_SMOOTH);
		}
	}


	if (gl_fogEnabled == true) {
		if (!glIsEnabled(GL_FOG)) {
			glEnable(GL_FOG);
		}
	}
	else {
		if (glIsEnabled(GL_FOG)) {
			glDisable(GL_FOG);
		}
	}
}
