#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "parson/parson.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>

#include "Assimp/include/cimport.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Component.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    modelScene = nullptr;
    modelMesh = nullptr;

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
    //RootParent->showingInHierarchy = false;
    AddGOList(RootParent);

    App->renderer3D->GameCam = App->scene->CreateGO("Cam", App->scene->RootParent);
    App->renderer3D->GameCam->CreateComp(Component::Types::CAMERA);
    App->renderer3D->MainCam = App->renderer3D->GameCam->GOcam;
   /* App->renderer3D->MainCam->cameraFrustum.pos =float3(0,5,-200);
    App->renderer3D->GameCam->GOtrans->SetPos({ 0,5,-20 });
    App->renderer3D->GameCam->GOtrans->SetRot({180,0,180});*/
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
    
    modelScene = nullptr;
    modelMesh = nullptr;
    house_loaded = false;
}

void ModuleScene::CreateGORoot(GameObject* obj, const char* name, GameObject* parent)
{

}

GameObject* ModuleScene::CreateGO( const char* name, GameObject* parent)
{

    GameObject* obj;
    obj = new GameObject(name, parent);
    
    AddGOList(obj);

    return obj;
}

void ModuleScene::AddGOList(GameObject* objlist)
{
    ListGO.push_back(objlist);
}


void ModuleScene::LoadSpecific(uint uid)
{
    for (int i = 0; i < App->scene->resourceList.size(); i++) {
        if (uid == resourceList[i]->assetID) {
            Resource* payload_res = App->scene->resourceList[i];
            ModelData* payload_model = (ModelData*)payload_res;
            if (payload_model->meshDatas.size() == 1) {
                LOG("Dropped %s in scene", payload_res->assetName.c_str());
                GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), ListGO[0]);


                go->CreateComp(Component::Types::MESH);

                char* fileBuffer = nullptr;
                std::string libName = std::to_string(payload_res->assetID) + ".chad";
                uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                MeshData* new_mesh_data = new MeshData(payload_res->assetName.c_str());
                MeshImporter::Load(fileBuffer, new_mesh_data);

                go->GOmesh->meshData = new_mesh_data;
                go->GOmesh->meshData->LoadBuffers();

                go->CreateComp(Component::Types::MATERIAL);
                if (payload_model->meshDatas[0]->materialAttachedID != 0) {

                    char* fileBuffer = nullptr;
                    std::string libName = std::to_string(payload_model->meshDatas[0]->materialAttachedID) + ".chad";

                    uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                    MaterialData* new_material_data = new MaterialData(payload_res->assetName.c_str());
                    MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

                    go->GOmat->materialData = new_material_data;
                }


            }
            else {

                LOG("Dropped %s in scene", payload_res->assetName.c_str());
                GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), ListGO[0]);

                for (int ind = 0; ind < payload_model->meshDatas.size(); ind++) {
                    GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
                    goChild->CreateComp(Component::Types::MESH);
                    goChild->GOmesh->meshData = payload_model->meshDatas[ind];
                    goChild->GOmesh->meshData->LoadBuffers();

                    goChild->CreateComp(Component::Types::MATERIAL);

                    if (goChild->GOmesh->meshData->materialAttachedID != 0) {

                        char* fileBuffer = nullptr;
                        std::string libName = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

                        uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                        MaterialData* new_material_data = new MaterialData(payload_res->assetName.c_str());
                        MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

                        goChild->GOmat->materialData = new_material_data;
                    }

                }
            }
        }
    }

}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

    for (int i = 0; i < models[0]->meshDatas.size(); i++)
    {
        models[0]->meshDatas[i]->UnloadMesh();
    }

    
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

bool ModuleScene::SaveSceneAtPlay() {

    //ImVec4 backgroundColor = App->editor->clear_color;
    int screenWidth = SDL_GetWindowSurface(App->window->window)->w, screenHeight = SDL_GetWindowSurface(App->window->window)->h;
    const char* AppName = App->EngName.c_str();
    const char* OrgName = App->OrgName.c_str();

    bool show_demo_window = App->editor->show_demo_window;
    bool show_console_window = App->editor->show_console_window;
    bool show_assets_window = App->editor->show_assets_window;
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

    std::vector <GameObject*> saveGoList;

    for (int i = 0; i < App->scene->ListGO.size(); i++) {
        if (i > 0) {
            saveGoList.push_back(App->scene->ListGO[i]);
        }
    }


    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("scene_at_play.json");

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
        json_object_set_boolean(json_object(scene_settings), "show_assets_window", show_assets_window);
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

        for (int i = 0; i < saveGoList.size(); i++) {
            if (i > 0) {
                std::string goName = "Gameobject [" + std::to_string(i) + "] Name";
                json_object_set_string(json_object(scene_settings), goName.c_str(), saveGoList[i]->name.c_str());
                std::string goUid = "Gameobject [" + std::to_string(i) + "] UID";
                json_object_set_number(json_object(scene_settings), goUid.c_str(), saveGoList[i]->uid);
                std::string goParent = "Gameobject [" + std::to_string(i) + "] Parent";
                json_object_set_number(json_object(scene_settings), goParent.c_str(), saveGoList[i]->parent->uid);
                std::string goComp;
                for (int j = 0; j < saveGoList[i]->components.size(); j++) {
                   
                    switch (saveGoList[i]->components[j]->type) {
                        case Component::Types::TRANSFORM:
                            {

                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->pos.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->pos.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->pos.z);

                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->rot.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->rot.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->rot.z);

                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->scl.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->scl.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOtrans->scl.z);
                            
                            }
                        break;
                        case Component::Types::MESH:
                            {
                                goComp = "Gameobject [" + std::to_string(i) + "] Mesh UID";
                                /*for (int k = 0; k < App->scene->resourceList.size(); k++) {
                                    if (App->scene->resourceList[k]->assetID == saveGoList[i]->GOmesh->meshData->assetID) {
                                        App->scene->LoadSpecific(App->scene->resourceList[k]->assetID);
                                    }

                                }*/
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetID);

                            }
                        break;
                        case Component::Types::MATERIAL:
                            {

                            }
                        break;
                        case Component::Types::CAMERA:
                            {

                            }
                        break;
                        case Component::Types::LIGHT:
                            {

                            }
                        break;
                   
                    };
                    
                }
            }
        }
        

        json_serialize_to_file(scene_settings, "Settings/scene_at_play.json");
    }

    json_serialize_to_file_pretty(scene_settings, "Settings/scene_at_play.json");

    json_value_free(schema);
    json_value_free(scene_settings);

    return true;
}

bool ModuleScene::SaveScene() {

    //ImVec4 backgroundColor = App->editor->clear_color;
    int screenWidth = SDL_GetWindowSurface(App->window->window)->w, screenHeight = SDL_GetWindowSurface(App->window->window)->h;
    const char* AppName = App->EngName.c_str();
    const char* OrgName = App->OrgName.c_str();

    bool show_demo_window = App->editor->show_demo_window;
    bool show_console_window = App->editor->show_console_window;
    bool show_assets_window = App->editor->show_assets_window;
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
        json_object_set_boolean(json_object(scene_settings), "show_assets_window", show_assets_window);
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
   
    json_serialize_to_file_pretty(scene_settings, "Settings/scene_settings.json");

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
    bool show_assets_window = false;
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
        show_assets_window = json_object_get_boolean(json_object(scene_settings), "show_assets_window");
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
    App->editor->show_assets_window = show_assets_window;
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


    json_serialize_to_file_pretty(scene_settings, "Settings/scene_settings.json");

    json_value_free(schema);
    json_value_free(scene_settings);

    return true;
}

