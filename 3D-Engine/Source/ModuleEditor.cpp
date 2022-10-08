#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include "SDL/include/SDL.h"

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

}

// Destructor
ModuleEditor::~ModuleEditor()
{
    logs.clear();
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

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    /*SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);*/
    gl_context = SDL_GL_CreateContext(App->window->window);
    SDL_GL_MakeCurrent(App->window->window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

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
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    App->scene->LoadScene();
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ///////[RANDOM NUMBER GENERATOR]
    /*int randNum = randomLCG.Int(1,3);
    LOG("%i", randNum);*/


    /*randNum.to_string();
    LOG(randNum.to_string());
    */
    //std::string numberString = std::to_string(randNum);
    
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
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    MenuBar();
    Console();
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
   // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(App->window->window);

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

    SDL_GL_DeleteContext(gl_context);

    logs.clear();

	return true;
}

bool ModuleEditor::MenuBar()
{
    //LOG("MenuBAR");
  // ImGui::Begin("Menu bar", NULL, ImGuiWindowFlags_MenuBar);
    /*if(ImGui::BeginMenuBar() )
    {   

    }*/
    if (ImGui::BeginMainMenuBar())
    {
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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Demo Window"))
        {
            //if (show_demo_window==true) show_demo_window = false;
            // else show_demo_window = true;
            ////if (ImGui::Checkbox)
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Close"))
        {
            close = false;


            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Windowads")) {
        ImGui::Text("Options");
        if (ImGui::CollapsingHeader("APP"))
         { 
            
            ImGui::Text("App name:");
            ImGui::SameLine();
            ImGui::TextColored({255,0,0,1},"%s",TITLE);

            ImGui::Text("Organization:");
            ImGui::SameLine();
            ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
         }
        if (ImGui::CollapsingHeader("Window"))
        {

            ImGui::Text("Brightness:");
            ImGui::SameLine();
            float brigth = SDL_GetWindowBrightness(App->window->window);
            if (ImGui::SliderFloat("Brightness",&brigth,0,1 ))
            {
                SDL_SetWindowBrightness(App->window->window,brigth);
            }
            
            ImGui::Text("Width:");
            ImGui::SameLine();
            float widht = App->window->screenWidth;
            if (ImGui::SliderFloat("Width", &widht, 20, 1000))
            {
                SDL_SetWindowSize(App->window->window, widht, App->window->screenHeight);
               // SDL_SetWindowBrightness(App->window->window, brigth);
            }
            ImGui::Text("Height:");
            ImGui::SameLine();
            float height = App->window->screenHeight;
            if (ImGui::SliderFloat("Height", &height, 20, 1000))
            {
                SDL_SetWindowSize(App->window->window, App->window->screenWidth, height);
                // SDL_SetWindowBrightness(App->window->window, brigth);
            }
        }
        if (ImGui::CollapsingHeader("Hardware"))
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
            ImGui::TextColored({ 255,0,0,1 }, "%s", ORGANIZATION);
            ImGui::Separator();//--------------
            ImGui::Text("GPU:");
            ImGui::SameLine();
            ImGui::TextColored({ 255,0,0,1 }, "vendor %s device %s",vendor);
            ImGui::Text("Brand:");
            ImGui::SameLine();
            ImGui::TextColored({ 255,0,0,1 }, "%s",renderer );
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
        
                    

    }
        ImGui::End();
   

   
     return true;
}
void ModuleEditor::AddLogs(const char* text)
{
    logs.appendf(text);
}

void  ModuleEditor::Console()
{
    
    if(ImGui::Begin("Console")) {

        ImGui::Text(logs.begin());

        
    }
    ImGui::End();
}