#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "parson/parson.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>

#include "Assimp/include/cimport.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    modelScene = nullptr;
    modelMesh = nullptr;
    models.push_back(new ModelData());

}

ModuleScene::~ModuleScene()
{
    
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading scene");
	bool ret = true;


    //CreateGORoot(RootParent,"RootParent",nullptr);
    RootParent = new GameObject("RootParent", nullptr);
    AddGOList(RootParent);

  //RootParent->components[1].
    //LoadCustom("Assets/BakerHouse.fbx", &meshes);
   /* LoadCube(meshes);
    LoadSphere(meshes);
    LoadPyramid(meshes);
    LoadCylinder(meshes);*/
   /* for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->LoadBuffers();
    }*/
	return ret;
}

void ModuleScene::CleanMeshes(std::vector<MeshData*>* meshesVec) {
    for (int i = 0; i < meshesVec->size(); i++)
    {
        meshesVec->at(i)->UnloadMesh();
    }
    meshesVec->erase(meshesVec->begin()), meshesVec->end();
    meshesVec->clear();
    meshesVec = nullptr;
    App->imp->ReleaseFile(modelScene);
    modelScene = nullptr;
    modelMesh = nullptr;
    house_loaded = false;
}

void ModuleScene::CreateGORoot(GameObject* obj, const char* name, GameObject* parent)
{
   /* obj = new GameObject(name,parent);
    AddGOList(obj);*/
}

GameObject* ModuleScene::CreateGO( const char* name, GameObject* parent)
{
    //new GameObject(name, parent);
    GameObject* obj;
    obj = new GameObject(name, parent);
    
    AddGOList(obj);

    return obj;
}

void ModuleScene::AddGOList(GameObject* objlist)
{
    ListGO.push_back(objlist);
}

void ModuleScene::LoadCustom(const char* path, std::vector<MeshData*>* meshesVec) {

    const aiScene* newScene;
    newScene = App->imp->LoadFile(path);
    
    App->imp->GetMeshDatas(newScene, meshesVec);//aqui obtienes la mesh
    aiReleaseImport(newScene);
    newScene = nullptr;
    delete newScene;

   /* if (house_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile(path);
        App->imp->GetMeshDatas(newScene, meshesVec);
        aiReleaseImport(newScene);
        App->editor->selectedShape = Shapes::NONE;
        house_loaded = true;
        newScene = nullptr;
        delete newScene;
    }*/
}
void ModuleScene::LoadCustomObj(const char* path, const char* name) {
    
    const aiScene* newScene;
    newScene = App->imp->LoadFile(path);
    //for(uint i=0;newScene)
   // GameObject* newGo = App->scene->CreateGO();
    
    if (newScene!=nullptr)
    {
        App->imp->GetMeshDatasObj(newScene, name);//aqui obtienes la mesh
        aiReleaseImport(newScene);
    }
    else if(newScene == nullptr)
    {
        LOG("IMPOSIBLE TO LOAD THIS OBJECT, TRY EXPORTING THE FBX WITH BLENDER");
    }
    newScene = nullptr;
    delete newScene;
    
    
}

void ModuleScene::LoadHouse(std::vector<MeshData*>* meshesVec) {

    if (house_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile("Assets/BakerHouse.fbx");
        App->imp->GetMeshDatas(newScene, meshesVec);
        aiReleaseImport(newScene);
        App->editor->selectedShape = Shapes::HOUSE;
        house_loaded = true;
        newScene = nullptr;
        delete newScene;
    }
}

void ModuleScene::LoadCube(std::vector<MeshData*>* meshesVec) {
    if (cube_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile("Assets/Cube.fbx");
        App->imp->GetMeshDatas(newScene, meshesVec);
        App->editor->selectedShape = Shapes::CUBE;
        cube_loaded = true;
        newScene = nullptr;
        delete newScene;
    }
}
void ModuleScene::LoadSphere(std::vector<MeshData*>* meshesVec) {
    if (sphere_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile("Assets/Sphere.fbx");
        App->imp->GetMeshDatas(newScene, meshesVec);
        App->editor->selectedShape = Shapes::SPHERE;
        sphere_loaded = true;
        newScene = nullptr;
        delete newScene;
    }
}
void ModuleScene::LoadPyramid(std::vector<MeshData*>* meshesVec) {
    if (pyramid_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile("Assets/Pyramid.fbx");
        App->imp->GetMeshDatas(newScene, meshesVec);
        App->editor->selectedShape = Shapes::PYRAMID;
        pyramid_loaded = true;
        newScene = nullptr;
        delete newScene;
    }
}
void ModuleScene::LoadCylinder(std::vector<MeshData*>* meshesVec) {
    if (cylinder_loaded == false) {
        const aiScene* newScene;
        newScene = App->imp->LoadFile("Assets/Cylinder.fbx");
        App->imp->GetMeshDatas(newScene, meshesVec);
        App->editor->selectedShape = Shapes::CYLINDER;
        cylinder_loaded = true;
        newScene = nullptr;
        delete newScene;
    }
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

    for (int i = 0; i < models[0]->meshes.size(); i++)
    {
        models[0]->meshes[i]->UnloadMesh();
    }

    App->imp->ReleaseFile(modelScene);
    modelScene = nullptr;
    delete modelScene;

    modelMesh = nullptr;
    delete modelMesh;

    for (uint i = 0; i < ListGO.size(); ++i)
    {
        delete ListGO[i];
        ListGO[i] = nullptr;
    }
    ListGO.clear();

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
    for (uint i=0; i<ListGO.size(); ++i)
    {
        ListGO[i]->Update(dt);
    }

	return UPDATE_CONTINUE;
}

bool ModuleScene::SaveScene() {

    //ImVec4 backgroundColor = App->editor->clear_color;
    int screenWidth = SDL_GetWindowSurface(App->window->window)->w, screenHeight = SDL_GetWindowSurface(App->window->window)->h;
    const char* AppName = App->EngName.c_str();
    const char* OrgName = App->OrgName.c_str();

    bool show_demo_window = App->editor->show_demo_window;
    bool show_console_window = App->editor->show_console_window;
    bool show_render3d_window = App->editor->show_render3d_window;
    bool show_config_window = App->editor->show_config_window;
    bool show_about_window = App->editor->show_about_window;

    bool gl_depthTestEnabled = App->editor->gl_depthTestEnabled;
    bool gl_cullFaceEnabled = App->editor->gl_cullFaceEnabled;
    bool gl_lightingEnabled = App->editor->gl_lightingEnabled;
    bool gl_colorMaterialEnabled = App->editor->gl_colorMaterialEnabled;
    bool gl_texture2dEnabled = App->editor->gl_texture2dEnabled;
    bool gl_lineSmoothEnabled = App->editor->gl_lineSmoothEnabled;
    bool gl_fogEnabled = App->editor->gl_fogEnabled;

    float fog_density = App->editor->fog_density;
    float fog_start = App->editor->fog_start;
    float fog_end = App->editor->fog_end;
    float fog_color[] = { App->editor->fog_color[0], App->editor->fog_color[1], App->editor->fog_color[2], App->editor->fog_color[3] };

    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("scene_settings.json");

    if (scene_settings == NULL || json_validate(schema, scene_settings) != JSONSuccess) {
        scene_settings = json_value_init_object();
        //json_object_set_number(json_object(scene_settings), "R", backgroundColor.x);
        //json_object_set_number(json_object(scene_settings), "G", backgroundColor.y);
        //json_object_set_number(json_object(scene_settings), "B", backgroundColor.z);
        //json_object_set_number(json_object(scene_settings), "A", backgroundColor.w);
        json_object_set_number(json_object(scene_settings), "screen_width", screenWidth);
        json_object_set_number(json_object(scene_settings), "screen_height", screenHeight);
        json_object_set_string(json_object(scene_settings), "engine_name", AppName);
        json_object_set_string(json_object(scene_settings), "org_name", AppName);

        json_object_set_boolean(json_object(scene_settings), "show_demo_window", show_demo_window);
        json_object_set_boolean(json_object(scene_settings), "show_console_window", show_console_window);
        json_object_set_boolean(json_object(scene_settings), "show_render3d_window", show_render3d_window);
        json_object_set_boolean(json_object(scene_settings), "show_config_window", show_config_window);
        json_object_set_boolean(json_object(scene_settings), "show_about_window", show_about_window);

        json_object_set_boolean(json_object(scene_settings), "gl_depthTestEnabled", gl_depthTestEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_cullFaceEnabled", gl_cullFaceEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_lightingEnabled", gl_lightingEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_colorMaterialEnabled", gl_colorMaterialEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_texture2dEnabled", gl_texture2dEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_lineSmoothEnabled", gl_lineSmoothEnabled);
        json_object_set_boolean(json_object(scene_settings), "gl_fogEnabled", gl_fogEnabled);

        json_object_set_number(json_object(scene_settings), "fog_density", fog_density);
        json_object_set_number(json_object(scene_settings), "fog_start", fog_start);
        json_object_set_number(json_object(scene_settings), "fog_end", fog_end);
        json_object_set_number(json_object(scene_settings), "fog_color_0", fog_color[0]);
        json_object_set_number(json_object(scene_settings), "fog_color_1", fog_color[1]);
        json_object_set_number(json_object(scene_settings), "fog_color_2", fog_color[2]);
        json_object_set_number(json_object(scene_settings), "fog_color_3", fog_color[3]);


        json_serialize_to_file(scene_settings, "Settings/scene_settings.json");
    }
   

    json_value_free(schema);
    json_value_free(scene_settings);

	return true;
}

///[REMEMBER TO CLEAN ALL THE INITIAL LOADSCENE CALLS WHEN THEY ARE NOT NECESSARY] (App->scene->Loadscene()...)

bool ModuleScene::LoadScene() {

    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("Settings/scene_settings.json");
    //ImVec4 new_backgroundColor;
    int new_winWidth = 480, new_winHeight = 480;
    float name = NULL;
    const char* AppName = "DefaultName";
    const char* OrgName = "DefaultName";

    bool show_demo_window = false;
    bool show_console_window = false;
    bool show_render3d_window = false;
    bool show_config_window = false;
    bool show_about_window = false;

    bool gl_depthTestEnabled = false;
    bool gl_cullFaceEnabled = false;
    bool gl_lightingEnabled = false;
    bool gl_colorMaterialEnabled = false;
    bool gl_texture2dEnabled = false;
    bool gl_lineSmoothEnabled = false;
    bool gl_fogEnabled = false;

    float fog_density = false;
    float fog_start = false;
    float fog_end = false;
    float fog_color[] = { 0,0,0,0 };


    if (scene_settings == NULL || json_validate(schema, scene_settings) != JSONSuccess) {
        //scene_settings = json_value_init_object();
        //new_backgroundColor.x = json_object_get_number(json_object(scene_settings), "R");
        //new_backgroundColor.y = json_object_get_number(json_object(scene_settings), "G");
        //new_backgroundColor.z = json_object_get_number(json_object(scene_settings), "B");
        //new_backgroundColor.w = json_object_get_number(json_object(scene_settings), "A");

        new_winWidth = json_object_get_number(json_object(scene_settings), "screen_width");
        new_winHeight = json_object_get_number(json_object(scene_settings), "screen_height");
        AppName = json_object_get_string(json_object(scene_settings), "engine_name");
        OrgName = json_object_get_string(json_object(scene_settings), "org_name");

        show_demo_window = json_object_get_boolean(json_object(scene_settings), "show_demo_window");
        show_console_window =  json_object_get_boolean(json_object(scene_settings), "show_console_window");
        show_render3d_window = json_object_get_boolean(json_object(scene_settings), "show_render3d_window");
        show_config_window = json_object_get_boolean(json_object(scene_settings), "show_config_window");
        show_about_window = json_object_get_boolean(json_object(scene_settings), "show_about_window");

        gl_depthTestEnabled = json_object_get_boolean(json_object(scene_settings), "gl_depthTestEnabled");
        gl_cullFaceEnabled = json_object_get_boolean(json_object(scene_settings), "gl_cullFaceEnabled");
        gl_lightingEnabled = json_object_get_boolean(json_object(scene_settings), "gl_lightingEnabled");
        gl_colorMaterialEnabled = json_object_get_boolean(json_object(scene_settings), "gl_colorMaterialEnabled");
        gl_texture2dEnabled = json_object_get_boolean(json_object(scene_settings), "gl_texture2dEnabled");
        gl_lineSmoothEnabled = json_object_get_boolean(json_object(scene_settings), "gl_lineSmoothEnabled");
        gl_fogEnabled = json_object_get_boolean(json_object(scene_settings), "gl_fogEnabled");

        fog_density = json_object_get_number(json_object(scene_settings), "fog_density");
        fog_start = json_object_get_number(json_object(scene_settings), "fog_start");
        fog_end = json_object_get_number(json_object(scene_settings), "fog_end");
        fog_color[0] = json_object_get_number(json_object(scene_settings), "fog_color_0");
        fog_color[1] = json_object_get_number(json_object(scene_settings), "fog_color_1");
        fog_color[2] = json_object_get_number(json_object(scene_settings), "fog_color_2");
        fog_color[3] = json_object_get_number(json_object(scene_settings), "fog_color_3");
        //LOG("W::::::::::%i", new_winWidth);
    }
    
    //App->editor->clear_color = new_backgroundColor;
    /*App->window->screenWidth = new_screenWidth;
    App->window->screenHeight = new_screenHeight;*/
    SDL_SetWindowSize(App->window->window, new_winWidth, new_winHeight);

   



    //App->EngName = AppName;
    //App->OrgName = OrgName;

    App->editor->SetAppName(AppName);
    App->editor->SetOrgName(OrgName);

    App->window->winWidth = new_winWidth;
    App->window->winHeight = new_winHeight;

    App->editor->show_demo_window = show_demo_window;
    App->editor->show_console_window = show_console_window;
    App->editor->show_render3d_window = show_render3d_window;
    App->editor->show_config_window = show_config_window;
    App->editor->show_about_window = show_about_window;

    App->editor->gl_depthTestEnabled = gl_depthTestEnabled;
    App->editor->gl_cullFaceEnabled = gl_cullFaceEnabled;
    App->editor->gl_lightingEnabled = gl_lightingEnabled;
    App->editor->gl_colorMaterialEnabled = gl_colorMaterialEnabled;
    App->editor->gl_texture2dEnabled = gl_texture2dEnabled;
    App->editor->gl_lineSmoothEnabled = gl_lineSmoothEnabled;
    App->editor->gl_fogEnabled = gl_fogEnabled;

    App->editor->fog_density = fog_density;
    App->editor->fog_start = fog_start;
    App->editor->fog_end = fog_end;
    App->editor->fog_color[0] = fog_color[0];
    App->editor->fog_color[1] = fog_color[1];
    App->editor->fog_color[2] = fog_color[2];
    App->editor->fog_color[3] = fog_color[3];

    json_value_free(schema);
    json_value_free(scene_settings);

    return true;
}

