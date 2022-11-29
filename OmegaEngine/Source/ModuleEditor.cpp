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

#include "MeshImporter.h"
#include "ModuleFileSystem.h"


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

	fog_density = 0;
	fog_start = 0;
	fog_end = 0;
	fog_index = 0;
	fog_coord = 0;
	fog_color[0] = 0;
	fog_color[1] = 0;
	fog_color[2] = 0;
	fog_color[3] = 0;
	selectedFogMode = 1;

	GOIndex = 0;
	showingGOIndex = 0;
	selection_mask = 2;

	ModuleScene::Shapes::NONE;
	selectedShape = ModuleScene::Shapes::NONE;

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
	   /* if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGuiIO& io = ImGui::GetIO(); (void)io;*/

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		//{
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
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//    ImGui::Text("Hello from another window!");
		//    if (ImGui::Button("Close Me"))
		//        show_another_window = false;
		//    ImGui::End();
		//}
		//App->scene->CreateGO();
	}
	if(ImGui::Begin("inspector"))
	{
		for (uint i = 0; i< App->scene->ListGO.size(); ++i)
		{
			if (GOIndex == i && GOIndex != 0 && i!=0)
			{
				App->scene->ListGO[i]->Editor();
			}
			
		}
		//ImGui::Text("aaaaaaaaaa");
	}
	ImGui::End();
	CheckGLCapabilities();
	//CheckShapes();
	GOList();
	if (App->input->CallClose)
	{
		CloseEngine();
	}
	//LOG("%i", App->scene->ListGO.size());

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

	if (ImGui::Begin("Scene View", &show_render3d_window)) {

		const char* modes[] = { "Normal", "Checkers", "Wireframe" };

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
			if (gl_fogEnabled == true) {

				if (ImGui::TreeNode("Fog Parameters"))
				{
					const char* fogModes[] = { "Linear", "Exponential", "Exponential 2" };

					if (ImGui::Button("Fog mode"))
						ImGui::OpenPopup("Fog mode");
					ImGui::SameLine();
					ImGui::TextUnformatted(selectedFogMode == -1 ? "<None>" : fogModes[selectedFogMode]);
					if (ImGui::BeginPopup("Fog mode"))
					{
						for (int i = 0; i < IM_ARRAYSIZE(fogModes); i++) {
							if (ImGui::Selectable(fogModes[i])) {
								selectedFogMode = i;

							}
						}

						ImGui::EndPopup();
					}


					switch (selectedFogMode) {
					case 0:
						ImGui::SliderFloat("Start", &fog_start, 0.0f, 500.0f, "%.2f");
						ImGui::SliderFloat("End", &fog_end, 0.0f, 500.0f, "%.2f");
						break;
					case 1:
						ImGui::SliderFloat("Density", &fog_density, 0.0f, 1.0f, "%.2f");
						break;
					case 2:
						ImGui::SliderFloat("Density", &fog_density, 0.0f, 1.0f, "%.2f");
						break;
					default:
						ImGui::SliderFloat("Density", &fog_density, 0.0f, 1.0f, "%.2f");
					}
					ImGui::ColorEdit4("Color", fog_color);
					ImGui::TreePop();
				}


				switch (selectedFogMode) {
				case 0:
					glFogf(GL_FOG_MODE, GL_LINEAR);
					break;
				case 1:
					glFogf(GL_FOG_MODE, GL_EXP);
					break;
				case 2:
					glFogf(GL_FOG_MODE, GL_EXP2);
					break;
				default:
					glFogf(GL_FOG_MODE, GL_EXP);
				}

				glFogf(GL_FOG_DENSITY, fog_density);
				glFogf(GL_FOG_START, fog_start);
				glHint(GL_FOG_HINT, GL_DONT_CARE);
				glFogf(GL_FOG_END, fog_end);
				glFogfv(GL_FOG_COLOR, fog_color);
				glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
			}



			ImGui::TreePop();
		}



		/*int renderX = ImGui::GetWindowPos().x;
		int renderY = ImGui::GetWindowPos().y;
		int renderW = ImGui::GetWindowWidth();
		int renderH = ImGui::GetWindowHeight();*/
		//App->renderer3D->OnResize(renderX, -renderY + renderH, renderW, renderH);

		//SDL_GetWindowSize(App->window->window, &winWidth, &winHeight);
		//App->renderer3D->OnResize(0, 0, winWidth, winHeight);
	}
	ImGui::End();
}

void ModuleEditor::AssetsWindow() {

	if (ImGui::Begin("Assets", &show_assets_window)) {

		
		for (int n = 0; n < App->scene->resourceList.size(); n++)
		{
			ImGui::PushID(n);
			if ((n % 3) != 0)
				ImGui::SameLine();
			ImGui::Button(App->scene->resourceList[n]->assetName.c_str(), ImVec2(110, 110));

			// Our buttons are both drag sources and drag targets here!
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				// Set payload to carry the index of our item (could be anything)
				ImGui::SetDragDropPayload("LOAD_ASSET_INTO_SCENE", &n, sizeof(int));
				//ImGui::SetDragDropPayload("LOAD_ASSET_INTO_SCENE", &App->scene->resourceList[n], sizeof(int));

				// Display preview (could be anything, e.g. when dragging an image we could decide to display
				// the filename and a small preview of the image, etc.)
				ImGui::Text("Load file into scene");

				ImGui::EndDragDropSource();
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip(App->scene->resourceList[n]->assetName.c_str());
			}
				
			
			ImGui::PopID();
		}

	}
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
			OpenWeb("https://github.com/Xavierlm11/Omega-Engine");
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

	if (show_assets_window) {

		AssetsWindow();
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
		if (ImGui::Checkbox("Assets Window", &show_assets_window))
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
		/*if (ImGui::Checkbox("House", &load_house_shape))
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

		}*/

		char* shapes[] = {"Cube" , "Sphere" , "Pyramid", "Cylinder" };

		if (ImGui::MenuItem(shapes[0]))
		{
			App->scene->LoadCustomObj("Assets/Cube.fbx", shapes[0]);
		}
		if (ImGui::MenuItem(shapes[1]))
		{
			App->scene->LoadCustomObj("Assets/Sphere.fbx", shapes[1]);
		}
		if (ImGui::MenuItem(shapes[2]))
		{
			App->scene->LoadCustomObj("Assets/Pyramid.fbx", shapes[2]);
		}
		if (ImGui::MenuItem(shapes[3]))
		{
			App->scene->LoadCustomObj("Assets/Cylinder.fbx", shapes[3]);
		}


		if (ImGui::Button("Clear Shapes"))
		{
			DeleteGo();
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
		ImGui::TextWrapped("Do you want to save before leaving?");
		ImGui::Text("");
		if (ImGui::Button("Yes"))
		{
			/*OpenWeb("https://images.cdn.circlesix.co/image/1/700/0/uploads/posts/2018/09/63125b0ed58a7f29b69b20bb066fd184.jpg");
			OpenWeb("https://youtu.be/OgZzUJud3Q4");*/
			App->scene->SaveScene();
			
			App->input->close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			App->input->close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
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
				App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
			}
			App->scene->LoadHouse(&App->scene->models[0]->meshDatas);
			for (int i = 0; i < App->scene->models[0]->meshDatas.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else 
	{
		if (App->scene->house_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
		}
	}

	if (load_cube_shape == true) {

		

		if (App->scene->cube_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::CUBE) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
			}
			App->scene->LoadCube(&App->scene->models[0]->meshDatas);
			for (int i = 0; i < App->scene->models[0]->meshDatas.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->cube_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
		}
	}

	if (load_sphere_shape == true) {

		

		if (App->scene->sphere_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::SPHERE) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
			}
			App->scene->LoadSphere(&App->scene->models[0]->meshDatas);
			for (int i = 0; i < App->scene->models[0]->meshDatas.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
			
	}
	else
	{
		if (App->scene->sphere_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
		}
	}

	if (load_pyramid_shape == true) {

		
		if (App->scene->pyramid_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::PYRAMID) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
			}

			App->scene->LoadPyramid(&App->scene->models[0]->meshDatas);
			for (int i = 0; i < App->scene->models[0]->meshDatas.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->pyramid_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
		}
	}

	if (load_cylinder_shape == true) {

	

		if (App->scene->cylinder_loaded == false) {
			if (selectedShape != ModuleScene::Shapes::NONE && selectedShape != ModuleScene::Shapes::CYLINDER) {
				App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
			}
			App->scene->LoadCylinder(&App->scene->models[0]->meshDatas);
			for (int i = 0; i < App->scene->models[0]->meshDatas.size(); i++)
			{
				//App->scene->models[0]->meshes[i]->LoadBuffers();
			}
		}
	}
	else
	{
		if (App->scene->cylinder_loaded == true) {
			App->scene->CleanMeshes(&App->scene->models[0]->meshDatas);
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

void ModuleEditor::GOList()
{

	if (ImGui::Begin("Game Object List"))
	{
		

		gameObjectsShowing.clear();
		for (int i = 0; i < App->scene->ListGO.size(); i++)
		{
			if (App->scene->ListGO[i]->showingInHierarchy == true) {
				gameObjectsShowing.push_back(App->scene->ListGO[i]);
			}
		}

		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
		static bool align_label_with_current_x_position = false;
		static bool test_drag_and_drop = false;

		if (align_label_with_current_x_position)
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());


		//selection_mask = (1 << 2);
		for (int i = 0; i < gameObjectsShowing.size(); i++)
		{
			ImGuiTreeNodeFlags node_flags = base_flags;

			//Root
			if (gameObjectsShowing[i]->parent == nullptr)
			{
				if (i == showingGOIndex) {
					node_flags |= ImGuiTreeNodeFlags_Selected;
				}
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
					showingGOIndex = i;
					for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
						if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
							GOIndex = ind;
						}
					}
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
					{
						//	/*IM_ASSERT(payload->DataSize == sizeof(int));
						//	int payload_n = *(const int*)payload->Data;*/
						int resource_ind = *(const int*)payload->Data;
						Resource *payload_res = App->scene->resourceList[resource_ind];
						if (payload_res->resourceType==Resource::Types::MODEL) {
							ModelData * payload_model = (ModelData*)payload_res;
							if (payload_model->meshDatas.size()==1) {
								LOG("Dropped %s in scene", payload_res->assetName.c_str());
								GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);
								
								
								go->CreateComp(Component::Types::MESH);
								go->GOmesh->meshData = payload_model->meshDatas[0];
								go->GOmesh->meshData->LoadBuffers();

								go->CreateComp(Component::Types::MATERIAL);
								if (payload_model->meshDatas[0]->material != nullptr) {
									go->GOmat->materialData = payload_model->meshDatas[0]->material;
								}
								
								
									/////////////////go->GOmesh->meshData = App->scene->modelList[resource_ind]->meshDatas[0];
								//char* fileBuffer = nullptr;
								//uint bufferSize = App->fileSystem->FileToBuffer(payload_res->assetName.c_str(), &fileBuffer);
								//MeshImporter::Load(fileBuffer, go->GOmesh->meshData);
							}
							else {
								LOG("Dropped %s in scene", payload_res->assetName.c_str());
								GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);

								for (int ind = 0; ind < payload_model->meshDatas.size(); ind++) {
									GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
									goChild->CreateComp(Component::Types::MESH);
									goChild->GOmesh->meshData = payload_model->meshDatas[ind];
									goChild->GOmesh->meshData->LoadBuffers();

									goChild->CreateComp(Component::Types::MATERIAL);
									
								}
								

							}
							
						}
						
						//for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
						//	if (App->scene->ListGO[ind] == gameObjectsShowing[k]) {
						//		GOIndex = ind;
						//	}
						//}
					}
					//LOG("Droppesssssssd");
					ImGui::EndDragDropTarget();
				}


				if (test_drag_and_drop && ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
					ImGui::Text("This is a drag and drop source");
					ImGui::EndDragDropSource();
				}
				if (i == showingGOIndex) {
					node_flags = base_flags;
				}
			}
			else {
				//Parents
				if (gameObjectsShowing[i]->parent->parent == nullptr && gameObjectsShowing[i]->children.size() > 0)
				{
					if (i == showingGOIndex) {
						node_flags |= ImGuiTreeNodeFlags_Selected;
					}
					bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
						showingGOIndex = i;
						for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
							if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
								GOIndex = ind;
							}
						}
					}

					if (test_drag_and_drop && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
						ImGui::Text("This is a drag and drop source");
						ImGui::EndDragDropSource();
					}
					if (i == showingGOIndex) {
						node_flags = base_flags;
					}
					//Childs
					if (node_open)
					{
						if (hasToMoveSelection == true) {
							if (showingGOIndex > i)
							{
								showingGOIndex += gameObjectsShowing[i]->children.size();
							}
							hasToMoveSelection = false;
						}

						for (int j = 0; j < gameObjectsShowing[i]->children.size(); j++) {
							if (gameObjectsShowing[i]->children[j]->showingInHierarchy == false) {
								gameObjectsShowing[i]->children[j]->showingInHierarchy = true;
							}
							else {

								for (int k = 0; k < gameObjectsShowing.size(); k++) {
									if (gameObjectsShowing[i]->children[j] == gameObjectsShowing[k]) {
										if (k == showingGOIndex) {
											node_flags |= ImGuiTreeNodeFlags_Selected;
										}
										node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
										ImGui::TreeNodeEx((void*)(intptr_t)k, node_flags, gameObjectsShowing[k]->name.c_str(), k);
										if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
											showingGOIndex = k;
											for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
												if (App->scene->ListGO[ind] == gameObjectsShowing[k]) {
													GOIndex = ind;
												}
											}
										}

										if (test_drag_and_drop && ImGui::BeginDragDropSource())
										{
											ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
											ImGui::Text("This is a drag and drop source");
											ImGui::EndDragDropSource();
										}
										if (k == showingGOIndex) {
											node_flags = base_flags;
										}
									}
								}
							}
						}
						ImGui::TreePop();
					}
					else {
						if (hasToMoveSelection == false) {
							if (showingGOIndex > i && showingGOIndex <= i + gameObjectsShowing[i]->children.size()) {
								showingGOIndex = i;
								GOIndex = i;
							}
							else if (showingGOIndex > i)
							{
								showingGOIndex -= gameObjectsShowing[i]->children.size();
							}
							hasToMoveSelection = true;
						}

						for (int j = 0; j < gameObjectsShowing[i]->children.size(); j++) {
							if (gameObjectsShowing[i]->children[j]->showingInHierarchy == true) {
								gameObjectsShowing[i]->children[j]->showingInHierarchy = false;
							}
						}
					}

				}
				//Direct childs
				else if (gameObjectsShowing[i]->parent->parent == nullptr)
				{
					if (i == showingGOIndex) {
						node_flags |= ImGuiTreeNodeFlags_Selected;
					}
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
					ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
						showingGOIndex = i;
						for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
							if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
								GOIndex = ind;
							}
						}
					}

					if (test_drag_and_drop && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
						ImGui::Text("This is a drag and drop source");
						ImGui::EndDragDropSource();
					}
					if (i == showingGOIndex) {
						node_flags = base_flags;
					}
				}


			}
			if (align_label_with_current_x_position)
				ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		}
		if (GOIndex == 0) {
			GOIndex = -1;
		}
		/*if (showingGOIndex == 0) {
			showingGOIndex = -1;
		}*/
		//LOG("GOIndex: %i", GOIndex);
		//LOG("ShowingGOIndex: %i", showingGOIndex);

		
	

	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
		{
			/*IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_n = *(const int*)payload->Data;*/
			//LOG("DROPPED");

		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();

}

void ModuleEditor::DeleteGo()
{
	

	/*if (App->scene->ListGO.size()>1) {
		for (uint i = 1; i < App->scene->ListGO.size(); ++i) {

			App->scene->ListGO.at(i)->~GameObject();
		}

	}
		App->scene->ListGO.erase(App->scene->ListGO.begin() + 1, App->scene->ListGO.end());*/
		
		for (uint i = 1; i < App->scene->ListGO.size(); ++i) {
			
				App->scene->ListGO.at(i)->Remove();
				App->scene->ListGO.erase(App->scene->ListGO.begin()+1, App->scene->ListGO.end());
				//App->scene->ListGO.at(i) = nullptr;
				//delete App->scene->ListGO.at(i);
				
		}
		/*for (uint i = 1; i < App->scene->ListGO.size(); ++i) {
			App->scene->ListGO[i] = nullptr;
			delete App->scene->ListGO[i];

		}
			App->scene->ListGO.clear();*/

}



//void ModuleEditor::HierarchyShowParent(ImGuiTreeNodeFlags node_flags) {
//
//
//	// Items 0..2 are Tree Node
//	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
//	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//		node_clicked = i;
//	if (test_drag_and_drop && ImGui::BeginDragDropSource())
//	{
//		ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
//		ImGui::Text("This is a drag and drop source");
//		ImGui::EndDragDropSource();
//	}
//	if (node_open)
//	{
//		ImGui::BulletText("Blah blah\nBlah Blah");
//		ImGui::TreePop();
//	}
//
//
//	
//}
//
//void ModuleEditor::HierarchyShowChild() {
//
//}

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
			//glFogf(GL_FOG_DENSITY, 0.5f);
		}
	}
	else {
		if (glIsEnabled(GL_FOG)) {
			glDisable(GL_FOG);
		}
	}
}
