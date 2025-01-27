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
#include "MaterialImporter.h"
#include "CCamera.h"
#include "ModelImporter.h"
#include "CPhysics.h"


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

	playPressed = false;
	pausePressed = false;

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

	GOIndex = -1;
	showingGOIndex = -1;
	assetSelelected = -1;
	selection_mask = 2;

	ModuleScene::Shapes::NONE;
	selectedShape = ModuleScene::Shapes::NONE;
	inspectorShowing = InspectorShow::NONE;

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

	if(ImGui::Begin("inspector"))
	{
		
			
			switch(inspectorShowing)
			{
				case InspectorShow::NONE:
				{
						
					break;
				}

				case InspectorShow::GAMEOBJECT:
				{
					for (uint i = 0; i < App->scene->ListGO.size(); ++i)
					{
						if (GOIndex == i && GOIndex != 0 && i != 0)
						{
							App->scene->ListGO[i]->Editor();

							bool hasMesh = false;
							bool hasMat = false;
							bool hasCam = false;
							bool hasPhys = false;

							for each (Component *comp in App->scene->ListGO[i]->components)
							{
								switch (comp->type)
								{
								case Component::Types::MESH:
									hasMesh = true;
									break;
								case Component::Types::MATERIAL:
									hasMat = true;
									break;
								case Component::Types::CAMERA:
									hasCam = true;
									break;
								case Component::Types::PHYSICS:
									hasPhys = true;
									break;
								}
								
							}
							
							vector<const char*> components;

							//string components_s = "";
							if (hasMesh == false) {
								components.push_back("Mesh");
							}
							if (hasMat == false) {
								components.push_back("Material");
							}
							if (hasCam == false) {
								components.push_back("Camera");
							}
							if (hasPhys == false) {
								components.push_back("Physics");
							}

							if (components.empty() == false) {

								ImGui::Text(" ");

								if (ImGui::Button("Add Component", ImVec2(140, 40)))
								{
									ImGui::OpenPopup("Components");
								}

								if (ImGui::BeginPopup("Components"))
								{
									for (int j = 0; j < components.size(); j++) {
										if (ImGui::Selectable(components[j])) {

											int a = -1;
											if (components[j] == "Mesh") {
												a = 0;
											}
											else if (components[j] == "Material")
											{
												a = 1;
											}
											else if (components[j] == "Camera")
											{
												a = 2;
											}
											else if (components[j] == "Physics")
											{
												a = 3;
											}


											switch (a) {
											case 0:
											{
												App->scene->ListGO[i]->CreateComp(Component::Types::MESH);
											}
											break;
											case 1:
											{
												App->scene->ListGO[i]->CreateComp(Component::Types::MATERIAL);
											}
											break;
											case 2:
											{
												App->scene->ListGO[i]->CreateComp(Component::Types::CAMERA);
											}
											break;
											case 3:
											{
												App->scene->ListGO[i]->CreateComp(Component::Types::PHYSICS);
												App->scene->ListGO[i]->GOphys->phys = App->physics;
											}
											break;
											}
										}
									}

									ImGui::EndPopup();
								}
							}

							
							

							ImGui::Text(" ");

							if (ImGui::Button("Delete GameObject", ImVec2(140, 40))) {
								selectedObj = nullptr;
								GameObject* goPtr = App->scene->ListGO[GOIndex];
								
								std::vector<int> goToDelete;

								for (int k = 0; k < App->scene->ListGO.size(); k++) {
									if (goPtr == App->scene->ListGO[k]) {

										goToDelete.push_back(k);

									}
								}

								for (int j = 0; j < goPtr->children.size();j++) {

									for (int k = 0; k < App->scene->ListGO.size(); k++) {

										if (goPtr->children[j] == App->scene->ListGO[k]) {

											goToDelete.push_back(k);

										}
									}
								}

								sort(goToDelete.begin(), goToDelete.end(), greater<int>());

								for (int j = 0; j < goToDelete.size(); j++) {
									GameObject* goToDel = App->scene->ListGO[goToDelete[j]];

									std::vector<int> meshesToDelete;

									App->scene->ListGO.erase(App->scene->ListGO.begin() + goToDelete[j]);
									//LOG("MESHLIST: %s", App->scene->)

									for (int k = 0; k < App->renderer3D->meshlist.size(); k++)
									{
										for (int l = 0; l < goToDel->components.size(); l++)
										{
											if (App->renderer3D->meshlist[k] == goToDel->components[l])
											{
												meshesToDelete.push_back(k);
											}
										}
									}

									sort(meshesToDelete.begin(), meshesToDelete.end(), greater<int>());

									for (int k = 0; k < meshesToDelete.size(); k++)
									{
										App->renderer3D->meshlist.erase(App->renderer3D->meshlist.begin() + meshesToDelete[k]);
									}

									goToDel->Remove();

									GOIndex = -1;
									showingGOIndex = -1;
								}
							}
						}
					}

					break;
				}

				case InspectorShow::ASSET:
				{
					ShowAssetInfo();
					break;
				}
					
			}
				
	}
	ImGui::End();
	CheckGLCapabilities();
	

	GOList();
	if (App->input->CallClose)
	{
		CloseEngine();
	}

	return UPDATE_CONTINUE;
}

void ModuleEditor::ShowAssetInfo() {

	if (assetSelelected <= -1) {
		return;
	}

	Resource* res = App->scene->resourceList[assetSelelected];


	ImGui::Text("");

	string textString = "Name:" + res->assetName;
	
	ImGui::BulletText(""); 
	ImGui::SameLine();
	ImGui::TextWrapped(textString.c_str());

	switch (res->resourceType) {

		case Resource::Types::MODEL:
		{
			textString = "Type: Model";
			break;
		}

		case Resource::Types::MATERIAL:
		{
			textString = "Type: Material";
			break;
		}
	}

	ImGui::Text(" ");
	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped(textString.c_str());

	textString = "Path:" + res->assetPath;

	ImGui::Text("");
	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped(textString.c_str());

	ImGui::Text(" ");
	if (ImGui::Button("Delete Asset", ImVec2(120, 60))) {
		textString = ASSETS_PATH;
		textString += res->fileName;
		App->scene->resourceList.erase(App->scene->resourceList.begin() + assetSelelected);
	
		assetSelelected =-1;
		

		if (remove(textString.c_str())==0) 
		{
			LOG("Asset removed");
		}
	}

}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Quitting ImGui Module");
	// Cleanup

	if (playPressed == true) {
		playPressed = false;

		DeleteGo();
		App->scene->LoadSceneAtPlay();

		std::string folder_name = "Settings/scene_at_play.json";

		if (remove(folder_name.c_str()) == 0)
		{
			LOG("Save At Play file removed");
		}


	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	login.clear();

	selectedShape = ModuleScene::Shapes::NONE;

	return true;
}

bool ModuleEditor::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		BarFile();
		BarWindows();
		BarShapes();
		BarPlay();
		BarPause();
		

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

		for (unsigned int i = 0; i < login.size(); ++i) {
			e = &login[i];

			ImGui::Text(e->c_str());
		}
		if (scroll) ImGui::SetScrollHereY(1.0f), scroll = false;
	}

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
	}
	ImGui::End();
}

void ModuleEditor::AssetsWindow() {

	if (ImGui::Begin("Assets", &show_assets_window)) {

		
		for (int n = 0; n < App->scene->resourceList.size(); n++)
		{
			ImGui::PushID(n);
			if ((n % 4) != 0)
				ImGui::SameLine();

			if (ImGui::Button(App->scene->resourceList[n]->fileName.c_str(), ImVec2(120, 120)) ){
				assetSelelected = n;
				inspectorShowing = InspectorShow::ASSET;
			}

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
				ImGui::SetTooltip(App->scene->resourceList[n]->fileName.c_str());
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

	DrawSceneViewport();
	if(App->renderer3D->MainCam!=nullptr)
		DrawGameViewport();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::DrawSceneViewport()
{
	if (ImGui::Begin("Scene"))
	{
		if (ImGui::IsWindowFocused())App->camera->IsWindow = true;
		else App->camera->IsWindow = false;
		//ImGui::iswindow
		ImVec2 ViewSize = ImGui::GetContentRegionAvail();
			AspRatioScene = (float)(ViewSize.x / ViewSize.y);
			App->camera->ScnCam->SetRatio(AspRatioScene) ;
		if ((ViewSize.x/ViewSize.y)!=AspRatioScene)
		{
			//App->camera->UpdateFrustum();

		}
		ImGui::Image((ImTextureID)App->camera->ScnCam->GetCCamBuffer(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
			{

				int resource_ind = *(const int*)payload->Data;
				Resource* payload_res = App->scene->resourceList[resource_ind];
				switch (payload_res->resourceType) {
				case Resource::Types::MODEL:
				{
					{
						ModelData* payload_model = (ModelData*)payload_res;
						if (payload_model->meshDatas.size() == 1) {
							LOG("Dropped %s in scene", payload_res->assetName.c_str());
							GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[0]);


							go->CreateComp(Component::Types::MESH);

							char* fileBuffer = nullptr;
							std::string libName = std::to_string(payload_res->assetID) + ".chad";
							uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
							MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
							new_mesh_data->assetID = payload_res->assetID;
							MeshImporter::Load(fileBuffer, new_mesh_data);

							go->GOmesh->meshData = new_mesh_data;
							go->GOmesh->meshData->LoadBuffers();

							go->CreateComp(Component::Types::MATERIAL);
							if (payload_model->meshDatas[0]->materialAttachedID != 0) {

								char* fileBuffer = nullptr;
								std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

								uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);


							
								for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
									if (App->scene->resourceList[rInd]->assetID == payload_model->meshDatas[0]->materialAttachedID) {

										MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
										MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

										go->GOmat->materialData = new_material_data;
										go->GOmat->materialData->assetID = payload_model->meshDatas[0]->materialAttachedID;
									}
								}

								
							}


						}
						else {

							LOG("Dropped %s in scene", payload_res->assetName.c_str());
							GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[0]);


							char* fileBufferMesh = nullptr;
							std::string libName = std::to_string(payload_res->assetID) + ".chad";
							uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

							ModelImporter::Load(fileBufferMesh, payload_model);


							for (int ind = 0; ind < payload_model->meshDatas.size(); ind++)
							{
								GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
								goChild->CreateComp(Component::Types::MESH);
								goChild->GOmesh->meshData = payload_model->meshDatas[ind];
								goChild->GOmesh->meshData->LoadBuffers();
								goChild->GOmesh->meshData->assetID = payload_res->assetID;
								goChild->GOmesh->meshData->indexInModel = ind;

								goChild->CreateComp(Component::Types::MATERIAL);

								if (goChild->GOmesh->meshData->materialAttachedID != 0) {
									
									char* fileBufferMat = nullptr;
									std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

									uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
									for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
										if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

											MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
											MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

											goChild->GOmat->materialData = new_material_data;
											goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
										}
									}
									//MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
									//MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

									//goChild->GOmat->materialData = new_material_data;
									//goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;

								}
							}
						}

						break;
					}
				}

				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();
}

void ModuleEditor::DrawGameViewport()
{
	if (ImGui::Begin("Game"))
	{
		//if (ImGui::IsWindowHovered())App->camera->IsWindow = true;
		//else App->camera->IsWindow = false;

		
		ImVec2 ViewSize = ImGui::GetContentRegionAvail();
		AspRatioGame = (float)(ViewSize.x / ViewSize.y);
		App->renderer3D->MainCam->SetRatio(AspRatioGame);
		if ((ViewSize.x / ViewSize.y) != AspRatioGame)
		{
			//App->camera->UpdateFrustum();

		}
		ImGui::Image((ImTextureID)App->renderer3D->MainCam->GetCCamBuffer(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
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
			DeleteGo();
			
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

void ModuleEditor::LoadShape(const char* resName) {

	ModelData* res = nullptr;
	for (int i = 0; i < App->scene->resourceList.size(); i++) {
		if (App->scene->resourceList[i]->assetName == resName) {
			res = (ModelData*)App->scene->resourceList[i];
		}
	}


	if (res != nullptr) {


		if (res->meshDatas.size() == 1) {
			
			GameObject* go = App->scene->CreateGO(res->assetName.c_str(), gameObjectsShowing[0]);


			go->CreateComp(Component::Types::MESH);

			char* fileBuffer = nullptr;
			std::string libName = std::to_string(res->assetID) + ".chad";
			uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
			MeshData* new_mesh_data = new MeshData(res->assetPath.c_str());
			new_mesh_data->assetID = res->assetID;
			MeshImporter::Load(fileBuffer, new_mesh_data);

			go->GOmesh->meshData = new_mesh_data;
			go->GOmesh->meshData->LoadBuffers();

			go->CreateComp(Component::Types::MATERIAL);
			if (res->meshDatas[0]->materialAttachedID != 0) {

				char* fileBuffer = nullptr;
				std::string libName = std::to_string(res->meshDatas[0]->materialAttachedID) + ".chad";

				uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
				for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
					if (App->scene->resourceList[rInd]->assetID == res->meshDatas[0]->materialAttachedID) {

						MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
						MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

						go->GOmat->materialData = new_material_data;
						go->GOmat->materialData->assetID = res->meshDatas[0]->materialAttachedID;
					}
				}
			}


		}
		else {

			LOG("Dropped %s in scene", res->assetName.c_str());
			GameObject* go = App->scene->CreateGO(res->assetName.c_str(), gameObjectsShowing[0]);


			char* fileBufferMesh = nullptr;
			std::string libName = std::to_string(res->assetID) + ".chad";
			uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

			ModelImporter::Load(fileBufferMesh, res);


			for (int ind = 0; ind < res->meshDatas.size(); ind++)
			{
				GameObject* goChild = App->scene->CreateGO(res->assetName.c_str(), go);
				goChild->CreateComp(Component::Types::MESH);
				goChild->GOmesh->meshData = res->meshDatas[ind];
				goChild->GOmesh->meshData->LoadBuffers();
				goChild->GOmesh->meshData->assetID = res->assetID;
				goChild->GOmesh->meshData->indexInModel = ind;

				goChild->CreateComp(Component::Types::MATERIAL);

				if (goChild->GOmesh->meshData->materialAttachedID != 0) {

					char* fileBufferMat = nullptr;
					std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

					uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
					for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
						if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

							MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
							MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

							goChild->GOmat->materialData = new_material_data;
							goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
						}
					}

				}
			}
		}
	}
}

void ModuleEditor::BarShapes() {

	if (ImGui::BeginMenu("Shapes"))
	{
		char* shapes[] = {"Cube" , "Sphere" , "Pyramid", "Cylinder" };

		if (ImGui::MenuItem(shapes[0]))
		{
			LoadShape(shapes[0]);
		}
		if (ImGui::MenuItem(shapes[1]))
		{
			LoadShape(shapes[1]);
		}
		if (ImGui::MenuItem(shapes[2]))
		{
			LoadShape(shapes[2]);
		}
		if (ImGui::MenuItem(shapes[3]))
		{
			LoadShape(shapes[3]);
		}


		if (ImGui::Button("Clear GameObjects"))
		{
			selectedObj = nullptr;
			DeleteGo();
		}

		ImGui::EndMenu();
	}

}

void ModuleEditor::BarPlay() {

	bool color = false;

	if (playPressed == true) {
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		color = true;
	}

	if (ImGui::Button("Play"))
	{
		if (playPressed == true) {
			//App->scene->vehicle->~PhysVehicle3D();
			App->physics->DeleteVehicle();
			App->scene->vehicle = nullptr;
			pausePressed = false;
			playPressed = false;

			DeleteGo();
			App->physics->isWorldOn = false;
			App->scene->LoadSceneAtPlay();

			std::string folder_name = "Settings/scene_at_play.json";

			if (remove(folder_name.c_str()) == 0)
			{
				LOG("Save At Play file removed");
			}

			
		}
		else {
			App->scene->CreateVehicle();
			playPressed = true;
			App->physics->isWorldOn = true;
			App->physics->SaveWorldTransforms();
			App->scene->SaveSceneAtPlay();
		}
	}

	if (color == true) {
		ImGui::PopStyleColor(3);
	}

	
}

void ModuleEditor::BarPause() {

	bool color = false;

	if (playPressed == true) {
		if (pausePressed == true) {

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			color = true;
		}
	}

	if (ImGui::Button("Pause"))
	{
		if (pausePressed == true) {

			pausePressed = false;
			App->physics->isWorldOn = true;

		}
		else {

			pausePressed = true;
			App->physics->isWorldOn = false;

		}
	}

	if (color == true) {
		ImGui::PopStyleColor(3);
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
		
		ImGui::Text("Height:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%ipx", App->window->winHeight);
		
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

	const GLubyte* vendor = glGetString(GL_VENDOR); 
	const GLubyte* renderer = glGetString(GL_RENDERER); 
														
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
		
		ImGui::Separator();//--------------
		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "vendor %s device %s", vendor);
		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,0,0,1 }, "%s", renderer);
		
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
	

	

void ModuleEditor::GOList()
{

	if (ImGui::Begin("Game Object List"))
	{
		
		if (hasToMoveSelection == true)
		{
			showingGOIndex = 0;
		}
		
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

		if (align_label_with_current_x_position) {
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
		}
			
		for (int i = 0; i < gameObjectsShowing.size(); i++)
		{
			ImGuiTreeNodeFlags node_flags = base_flags;

			//Root
			if (gameObjectsShowing[i]->parent == nullptr)
			{
				if (GOIndex >= 0) {
					if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
					{
						node_flags |= ImGuiTreeNodeFlags_Selected;
					}
				}
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
					showingGOIndex = i;
					for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
						if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
							GOIndex = ind;
							selectedObj = App->scene->ListGO[ind];
						}
					}
					inspectorShowing = InspectorShow::GAMEOBJECT;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
					{

						int resource_ind = *(const int*)payload->Data;
						Resource* payload_res = App->scene->resourceList[resource_ind];
						switch (payload_res->resourceType) {
						case Resource::Types::MODEL:
						{
							{
								ModelData* payload_model = (ModelData*)payload_res;
								if (payload_model->meshDatas.size() == 1) {
									LOG("Dropped %s in scene", payload_res->assetName.c_str());
									GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


									go->CreateComp(Component::Types::MESH);

									char* fileBuffer = nullptr;
									std::string libName = std::to_string(payload_res->assetID) + ".chad";
									uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
									MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
									new_mesh_data->assetID = payload_res->assetID;
									MeshImporter::Load(fileBuffer, new_mesh_data);

									go->GOmesh->meshData = new_mesh_data;
									go->GOmesh->meshData->LoadBuffers();

									go->CreateComp(Component::Types::MATERIAL);
									if (payload_model->meshDatas[0]->materialAttachedID != 0) {

										char* fileBuffer = nullptr;
										std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

										uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
										for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
											if (App->scene->resourceList[rInd]->assetID == payload_model->meshDatas[0]->materialAttachedID) {

												MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
												MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

												go->GOmat->materialData = new_material_data;
												go->GOmat->materialData->assetID = payload_model->meshDatas[0]->materialAttachedID;
											}
										}
									
									}


								}
								else {

									LOG("Dropped %s in scene", payload_res->assetName.c_str());
									GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


									char* fileBufferMesh = nullptr;
									std::string libName = std::to_string(payload_res->assetID) + ".chad";
									uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);
										
									ModelImporter::Load(fileBufferMesh, payload_model);


									for (int ind = 0; ind < payload_model->meshDatas.size(); ind++)
									{
										GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
										goChild->CreateComp(Component::Types::MESH);
										goChild->GOmesh->meshData = payload_model->meshDatas[ind];
										goChild->GOmesh->meshData->LoadBuffers();
										goChild->GOmesh->meshData->assetID = payload_res->assetID;
										goChild->GOmesh->meshData->indexInModel = ind;

										goChild->CreateComp(Component::Types::MATERIAL);

										if (goChild->GOmesh->meshData->materialAttachedID != 0) {

											char* fileBufferMat = nullptr;
											std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

											uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
											for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
												if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

													MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
													MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

													goChild->GOmat->materialData = new_material_data;
													goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
												}
											}
											

										}
									}
								}

								break;
							}
						}

						}








					}
					ImGui::EndDragDropTarget();
				}

				if (test_drag_and_drop && ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
					ImGui::Text("This is a drag and drop source");
					ImGui::EndDragDropSource();
				}
					
				if (GOIndex >= 0) {
					if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
					{
						node_flags = base_flags;
					}
				}

			}
			else {
				//Parents
				if (gameObjectsShowing[i]->parent->parent == nullptr && gameObjectsShowing[i]->children.size() > 0)
				{
					if (GOIndex >= 0) {
						if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
						{
							node_flags |= ImGuiTreeNodeFlags_Selected;
						}
					}
					std::string objID = std::to_string(i);
					bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
						showingGOIndex = i;
						for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
							if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
								GOIndex = ind;
								selectedObj = App->scene->ListGO[ind];
							}
						}
						inspectorShowing = InspectorShow::GAMEOBJECT;
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
						{
							int resource_ind = *(const int*)payload->Data;
							Resource* payload_res = App->scene->resourceList[resource_ind];
							switch (payload_res->resourceType) {
							case Resource::Types::MODEL:
							{
								{
									ModelData* payload_model = (ModelData*)payload_res;
									if (payload_model->meshDatas.size() == 1) {
										LOG("Dropped %s in scene", payload_res->assetName.c_str());
										GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


										go->CreateComp(Component::Types::MESH);

										char* fileBuffer = nullptr;
										std::string libName = std::to_string(payload_res->assetID) + ".chad";
										uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
										MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
										new_mesh_data->assetID = payload_res->assetID;
										MeshImporter::Load(fileBuffer, new_mesh_data);

										go->GOmesh->meshData = new_mesh_data;
										go->GOmesh->meshData->LoadBuffers();

										go->CreateComp(Component::Types::MATERIAL);
										if (payload_model->meshDatas[0]->materialAttachedID != 0) {

											char* fileBuffer = nullptr;
											std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

											uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
											for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
												if (App->scene->resourceList[rInd]->assetID == payload_model->meshDatas[0]->materialAttachedID) {

													MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
													MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

													go->GOmat->materialData = new_material_data;
													go->GOmat->materialData->assetID = payload_model->meshDatas[0]->materialAttachedID;
												}
											}
										}


									}
									else {

										LOG("Dropped %s in scene", payload_res->assetName.c_str());
										GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


										char* fileBufferMesh = nullptr;
										std::string libName = std::to_string(payload_res->assetID) + ".chad";
										uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

										ModelImporter::Load(fileBufferMesh, payload_model);


										for (int ind = 0; ind < payload_model->meshDatas.size(); ind++)
										{
											GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
											goChild->CreateComp(Component::Types::MESH);
											goChild->GOmesh->meshData = payload_model->meshDatas[ind];
											goChild->GOmesh->meshData->LoadBuffers();
											goChild->GOmesh->meshData->assetID = payload_res->assetID;
											goChild->GOmesh->meshData->indexInModel = ind;

											goChild->CreateComp(Component::Types::MATERIAL);

											if (goChild->GOmesh->meshData->materialAttachedID != 0) {

												char* fileBufferMat = nullptr;
												std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

												uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
												for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
													if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

														MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
														MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

														goChild->GOmat->materialData = new_material_data;
														goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
													}
												}

											}
										}
									}

									break;
								}
							}
							case Resource::Types::MATERIAL:
							{

								gameObjectsShowing[i]->CreateComp(Component::Types::MATERIAL);

								char* fileBuffer = nullptr;
								std::string libName = std::to_string(payload_res->assetID) + ".chad";

								uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
								MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
								MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

								gameObjectsShowing[i]->GOmat->materialData = new_material_data;
								gameObjectsShowing[i]->GOmat->materialData->assetID = payload_res->assetID;


								break;
							}
							}

						}
						ImGui::EndDragDropTarget();
					}

					if (test_drag_and_drop && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
						ImGui::Text("This is a drag and drop source");
						ImGui::EndDragDropSource();
					}
					
					if (GOIndex >= 0) {
						if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
						{
							node_flags = base_flags;
						}
					}
					//Childs
					if (node_open)
					{

						for (int j = 0; j < gameObjectsShowing[i]->children.size(); j++) {

							if (gameObjectsShowing[i]->children[j]->showingInHierarchy == false) {
								gameObjectsShowing[i]->children[j]->showingInHierarchy = true;
							}
							else {

								for (int k = 0; k < gameObjectsShowing.size(); k++) {
									if (gameObjectsShowing[i]->children[j] == gameObjectsShowing[k]) {
										if (GOIndex >= 0) {
											if (gameObjectsShowing[k] == App->scene->ListGO[GOIndex])
											{
												node_flags |= ImGuiTreeNodeFlags_Selected;
											}
										}
										node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
										ImGui::TreeNodeEx((void*)(intptr_t)k, node_flags, gameObjectsShowing[k]->name.c_str(), k);
										if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
											showingGOIndex = k;
											for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
												if (App->scene->ListGO[ind] == gameObjectsShowing[k]) {
													GOIndex = ind;
													selectedObj = App->scene->ListGO[ind];
												}
											}
											inspectorShowing = InspectorShow::GAMEOBJECT;
										}

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
											{
												int resource_ind = *(const int*)payload->Data;
												Resource* payload_res = App->scene->resourceList[resource_ind];
												switch (payload_res->resourceType) {
												case Resource::Types::MODEL:
												{
													{
														ModelData* payload_model = (ModelData*)payload_res;
														if (payload_model->meshDatas.size() == 1) {
															LOG("Dropped %s in scene", payload_res->assetName.c_str());
															GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


															go->CreateComp(Component::Types::MESH);

															char* fileBuffer = nullptr;
															std::string libName = std::to_string(payload_res->assetID) + ".chad";
															uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
															MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
															new_mesh_data->assetID = payload_res->assetID;
															MeshImporter::Load(fileBuffer, new_mesh_data);

															go->GOmesh->meshData = new_mesh_data;
															go->GOmesh->meshData->LoadBuffers();

															go->CreateComp(Component::Types::MATERIAL);
															if (payload_model->meshDatas[0]->materialAttachedID != 0) {

																char* fileBuffer = nullptr;
																std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

																uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
																for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
																	if (App->scene->resourceList[rInd]->assetID == payload_model->meshDatas[0]->materialAttachedID) {

																		MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
																		MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

																		go->GOmat->materialData = new_material_data;
																		go->GOmat->materialData->assetID = payload_model->meshDatas[0]->materialAttachedID;
																	}
																}
															}


														}
														else {

															LOG("Dropped %s in scene", payload_res->assetName.c_str());
															GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


															char* fileBufferMesh = nullptr;
															std::string libName = std::to_string(payload_res->assetID) + ".chad";
															uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

															ModelImporter::Load(fileBufferMesh, payload_model);


															for (int ind = 0; ind < payload_model->meshDatas.size(); ind++)
															{
																GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
																goChild->CreateComp(Component::Types::MESH);
																goChild->GOmesh->meshData = payload_model->meshDatas[ind];
																goChild->GOmesh->meshData->LoadBuffers();
																goChild->GOmesh->meshData->assetID = payload_res->assetID;
																goChild->GOmesh->meshData->indexInModel = ind;

																goChild->CreateComp(Component::Types::MATERIAL);

																if (goChild->GOmesh->meshData->materialAttachedID != 0) {

																	char* fileBufferMat = nullptr;
																	std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

																	uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
																	for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
																		if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

																			MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
																			MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

																			goChild->GOmat->materialData = new_material_data;
																			goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
																		}
																	}

																}
															}
														}

														break;
													}
												}
												case Resource::Types::MATERIAL:

												{

													gameObjectsShowing[k]->CreateComp(Component::Types::MATERIAL);

													char* fileBuffer = nullptr;
													std::string libName = std::to_string(payload_res->assetID) + ".chad";

													uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
													MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
													MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

													gameObjectsShowing[k]->GOmat->materialData = new_material_data;
													gameObjectsShowing[k]->GOmat->materialData->assetID = payload_res->assetID;


													break;
												}
												}

											}
											ImGui::EndDragDropTarget();
										}

										if (test_drag_and_drop && ImGui::BeginDragDropSource())
										{
											ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
											ImGui::Text("This is a drag and drop source");
											ImGui::EndDragDropSource();
										}
										
										if (GOIndex >= 0) {
											if (gameObjectsShowing[k] == App->scene->ListGO[GOIndex])
											{
												node_flags = base_flags;
											}
										}
										
									}
								}
							}
						}

						ImGui::TreePop();
					}
					else 
					{
						
					}

				}
				//Direct childs
				else if (gameObjectsShowing[i]->parent->parent == nullptr)
				{
				if (GOIndex >= 0) {
					if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
					{
						node_flags |= ImGuiTreeNodeFlags_Selected;
					}
				}
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
					ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObjectsShowing[i]->name.c_str(), i);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
						showingGOIndex = i;
						for (int ind = 0; ind < App->scene->ListGO.size(); ind++) {
							if (App->scene->ListGO[ind] == gameObjectsShowing[i]) {
								GOIndex = ind;
								selectedObj = App->scene->ListGO[ind];
							}
						}
						inspectorShowing = InspectorShow::GAMEOBJECT;
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LOAD_ASSET_INTO_SCENE"))
						{

							int resource_ind = *(const int*)payload->Data;
							Resource* payload_res = App->scene->resourceList[resource_ind];
							switch (payload_res->resourceType) {
							case Resource::Types::MODEL:
							{
								{
									ModelData* payload_model = (ModelData*)payload_res;
									if (payload_model->meshDatas.size() == 1) {
										LOG("Dropped %s in scene", payload_res->assetName.c_str());
										GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


										go->CreateComp(Component::Types::MESH);

										char* fileBuffer = nullptr;
										std::string libName = std::to_string(payload_res->assetID) + ".chad";
										uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
										MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
										new_mesh_data->assetID = payload_res->assetID;
										MeshImporter::Load(fileBuffer, new_mesh_data);

										go->GOmesh->meshData = new_mesh_data;
										go->GOmesh->meshData->LoadBuffers();

										go->CreateComp(Component::Types::MATERIAL);
										if (payload_model->meshDatas[0]->materialAttachedID != 0) {

											char* fileBuffer = nullptr;
											std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

											uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
											for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
												if (App->scene->resourceList[rInd]->assetID == payload_model->meshDatas[0]->materialAttachedID) {

													MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
													MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

													go->GOmat->materialData = new_material_data;
													go->GOmat->materialData->assetID = payload_model->meshDatas[0]->materialAttachedID;
												}
											}
										}


									}
									else {

										LOG("Dropped %s in scene", payload_res->assetName.c_str());
										GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), gameObjectsShowing[i]);


										char* fileBufferMesh = nullptr;
										std::string libName = std::to_string(payload_res->assetID) + ".chad";
										uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

										ModelImporter::Load(fileBufferMesh, payload_model);


										for (int ind = 0; ind < payload_model->meshDatas.size(); ind++)
										{
											GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
											goChild->CreateComp(Component::Types::MESH);
											goChild->GOmesh->meshData = payload_model->meshDatas[ind];
											goChild->GOmesh->meshData->LoadBuffers();
											goChild->GOmesh->meshData->assetID = payload_res->assetID;
											goChild->GOmesh->meshData->indexInModel = ind;

											goChild->CreateComp(Component::Types::MATERIAL);

											if (goChild->GOmesh->meshData->materialAttachedID != 0) {

												char* fileBufferMat = nullptr;
												std::string libNameMat = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

												uint bufferSizeMat = App->fileSystem->FileToBuffer(libNameMat.c_str(), &fileBufferMat);
												for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
													if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

														MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
														MaterialImporter::Load(fileBufferMat, new_material_data, bufferSizeMat);

														goChild->GOmat->materialData = new_material_data;
														goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
													}
												}

											}
										}
									}

									break;
								}
							}
							case Resource::Types::MATERIAL:
							{


								gameObjectsShowing[i]->CreateComp(Component::Types::MATERIAL);

								char* fileBuffer = nullptr;
								std::string libName = std::to_string(payload_res->assetID) + ".chad";



								uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
								MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
								MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

								gameObjectsShowing[i]->GOmat->materialData = new_material_data;
								gameObjectsShowing[i]->GOmat->materialData->assetID = payload_res->assetID;


								break;
							}
							}
						}
						ImGui::EndDragDropTarget();
					}

					if (test_drag_and_drop && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
						ImGui::Text("This is a drag and drop source");
						ImGui::EndDragDropSource();
					}

					if (GOIndex >= 0) {
						if (gameObjectsShowing[i] == App->scene->ListGO[GOIndex])
						{
							node_flags = base_flags;
						}
					}
				}


			}
			if (align_label_with_current_x_position) {

				ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
			}
		}
		if (GOIndex == 0) {
			GOIndex = -1;
		}

		//LOG("GOIndex: %i", GOIndex);
		//LOG("ShowingGOIndex: %i", showingGOIndex);

		if (hasToMoveSelection == true)
		{
			showingGOIndex = 0;
			hasToMoveSelection = false;
		}


	}

	ImGui::End();

}

void ModuleEditor::DeleteGo()
{
	for (uint i = 2; i < App->scene->ListGO.size(); i++) {
			
			//if(App->scene->ListGO[i]->GOcam==nullptr)
			//{
				App->scene->ListGO[i]->Remove();
				
			//}
				//App->scene->ListGO.at(i) = nullptr;
				//delete App->scene->ListGO.at(i);
				
	}

	App->scene->ListGO.erase(App->scene->ListGO.begin() + 2, App->scene->ListGO.end());
	App->editor->selectedObj = nullptr;
	App->editor->GOIndex = 0;
	//App->editor->sele
		/*for (uint i = 1; i < App->scene->ListGO.size(); ++i) {
			App->scene->ListGO[i] = nullptr;
			delete App->scene->ListGO[i];

		}
			App->scene->ListGO.clear();*/

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


void ModuleEditor::CreateSphere(const char* resName,float force ) {

	ModelData* res = nullptr;
	for (int i = 0; i < App->scene->resourceList.size(); i++) {
		if (App->scene->resourceList[i]->assetName == resName) {
			res = (ModelData*)App->scene->resourceList[i];
		}
	}


	if (res != nullptr) {


		if (res->meshDatas.size() == 1) {

			GameObject* go = App->scene->CreateGO(res->assetName.c_str(), gameObjectsShowing[0]);


			go->CreateComp(Component::Types::MESH);

			char* fileBuffer = nullptr;
			std::string libName = std::to_string(res->assetID) + ".chad";
			uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
			MeshData* new_mesh_data = new MeshData(res->assetPath.c_str());
			new_mesh_data->assetID = res->assetID;
			MeshImporter::Load(fileBuffer, new_mesh_data);

			go->GOmesh->meshData = new_mesh_data;
			go->GOmesh->meshData->LoadBuffers();

			go->CreateComp(Component::Types::MATERIAL);
			if (res->meshDatas[0]->materialAttachedID != 0) {

				char* fileBuffer = nullptr;
				std::string libName = std::to_string(res->meshDatas[0]->materialAttachedID) + ".chad";

				uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
				for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
					if (App->scene->resourceList[rInd]->assetID == res->meshDatas[0]->materialAttachedID) {

						MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
						MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

						go->GOmat->materialData = new_material_data;
						go->GOmat->materialData->assetID = res->meshDatas[0]->materialAttachedID;
					}
				}
			}
			go->GOtrans->SetPos(App->renderer3D->GameCam->GOtrans->GetPos());
			go->CreateComp(Component::Types::PHYSICS);
			go->GOphys->phys = App->physics;
			go->GOphys->shapeSelected = CPhysics::ColliderShape::SPHERE;
			go->GOphys->isStatic = false;
			go->GOphys->isShapeSelected[1] = true;
			// App->renderer3D->GameCam->GOphys->CheckShapes();
			if (go->GOphys->shapeSelected != CPhysics::ColliderShape::NONE) {
				CCamera* Cam = App->renderer3D->GameCam->GOcam;
				//App->renderer3D->GameCam->GOphys->colPos = App->renderer3D->GameCam->GOcam->cameraFrustum.pos;
				go->GOphys->colPos = App->renderer3D->GameCam->GOtrans->GetPos();
				go->GOphys->sphereRadius = 0.6f;
				go->GOphys->CreateCollider();
				go->GOphys->CallUpdateShape();
				go->GOphys->collider->Push(-(Cam->Z.x * force), -(Cam->Z.y * force), -(Cam->Z.z * force));
			}
			go->name = "TemporalObject";

		}
		
	}
}