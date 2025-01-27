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
#include "ModelImporter.h"
#include "ModelData.h"
#include "PhysVehicle3D.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    modelScene = nullptr;
    modelMesh = nullptr;
    vehicle = nullptr;
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

    App->renderer3D->GameCam = App->scene->CreateGO("GameCam", App->scene->RootParent);
    App->renderer3D->GameCam->CreateComp(Component::Types::CAMERA);
    App->renderer3D->GameCam->CreateComp(Component::Types::PHYSICS);
    App->renderer3D->GameCam->GOphys->phys = App->physics;
    App->renderer3D->GameCam->GOphys->shapeSelected = CPhysics::ColliderShape::SPHERE;
    App->renderer3D->GameCam->GOphys->isStatic = false;
    App->renderer3D->GameCam->GOphys->isShapeSelected[1] = true;
    // App->renderer3D->GameCam->GOphys->CheckShapes();
    if (App->renderer3D->GameCam->GOphys->shapeSelected != CPhysics::ColliderShape::NONE) {

        //App->renderer3D->GameCam->GOphys->colPos = App->renderer3D->GameCam->GOcam->cameraFrustum.pos;
        App->renderer3D->GameCam->GOphys->colPos = App->renderer3D->GameCam->GOtrans->GetPos();
        App->renderer3D->GameCam->GOphys->sphereRadius = 0.6f;
        App->renderer3D->GameCam->GOphys->CreateCollider();
        App->renderer3D->GameCam->GOphys->CallUpdateShape();
    }
    App->renderer3D->MainCam = App->renderer3D->GameCam->GOcam;
    App->renderer3D->GameCam->GOcam->ismain = true;

  

    //CreateVehicle();

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

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

void ModuleScene::CreateVehicle()
{
    VehicleInfo car;

    // Car properties ----------------------------------------
    car.chassis_size.Set(2, 2, 4);
    car.chassis_offset.Set(0, 1.5, 0);
    car.mass = 500.0f;
    car.suspensionStiffness = 15.88f;
    car.suspensionCompression = 0.83f;
    car.suspensionDamping = 0.88f;
    car.maxSuspensionTravelCm = 1000.0f;
    car.frictionSlip = 50.5;
    car.maxSuspensionForce = 6000.0f;

    // Wheel properties ---------------------------------------
    float connection_height = 1.2f;
    float wheel_radius = 0.6f;
    float wheel_width = 0.5f;
    float suspensionRestLength = 1.2f;

    // Don't change anything below this line ------------------

    float half_width = car.chassis_size.x * 0.5f;
    float half_length = car.chassis_size.z * 0.5f;

    vec3 direction(0, -1, 0);
    vec3 axis(-1, 0, 0);

    car.num_wheels = 4;
    car.wheels = new Wheel[4];

    // FRONT-LEFT ------------------------
    car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
    car.wheels[0].direction = direction;
    car.wheels[0].axis = axis;
    car.wheels[0].suspensionRestLength = suspensionRestLength;
    car.wheels[0].radius = wheel_radius;
    car.wheels[0].width = wheel_width;
    car.wheels[0].front = true;
    car.wheels[0].drive = true;
    car.wheels[0].brake = false;
    car.wheels[0].steering = true;

    // FRONT-RIGHT ------------------------
    car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
    car.wheels[1].direction = direction;
    car.wheels[1].axis = axis;
    car.wheels[1].suspensionRestLength = suspensionRestLength;
    car.wheels[1].radius = wheel_radius;
    car.wheels[1].width = wheel_width;
    car.wheels[1].front = true;
    car.wheels[1].drive = true;
    car.wheels[1].brake = false;
    car.wheels[1].steering = true;

    // REAR-LEFT ------------------------
    car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
    car.wheels[2].direction = direction;
    car.wheels[2].axis = axis;
    car.wheels[2].suspensionRestLength = suspensionRestLength;
    car.wheels[2].radius = wheel_radius;
    car.wheels[2].width = wheel_width;
    car.wheels[2].front = false;
    car.wheels[2].drive = false;
    car.wheels[2].brake = true;
    car.wheels[2].steering = false;

    // REAR-RIGHT ------------------------
    car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
    car.wheels[3].direction = direction;
    car.wheels[3].axis = axis;
    car.wheels[3].suspensionRestLength = suspensionRestLength;
    car.wheels[3].radius = wheel_radius;
    car.wheels[3].width = wheel_width;
    car.wheels[3].front = false;
    car.wheels[3].drive = false;
    car.wheels[3].brake = true;
    car.wheels[3].steering = false;

    vehicle = App->physics->AddVehicle(car);
    vehicle->SetPos(0, 12, 10);
}

void ModuleScene::ControlVehicle()
{
    if (vehicle == nullptr) {
        return;
    }
    turn = acceleration = brake = 0.0f;

    if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
    {
        acceleration = MAX_ACCELERATION;
    }

    if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
    {
        if (turn < TURN_DEGREES)
            turn += TURN_DEGREES;
    }

    if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
    {
        if (turn > -TURN_DEGREES)
            turn -= TURN_DEGREES;
    }

    if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
    {
        brake = BRAKE_POWER;
    }

    vehicle->ApplyEngineForce(acceleration);
    vehicle->Turn(turn);
    vehicle->Brake(brake);

    vehicle->Render();

   // char title[80];
    //sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
    //App->window->SetTitle(title);

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
                MeshData* new_mesh_data = new MeshData(payload_res->assetPath.c_str());
                MeshImporter::Load(fileBuffer, new_mesh_data);

                go->GOmesh->meshData = new_mesh_data;
                go->GOmesh->meshData->LoadBuffers();
                go->GOmesh->meshData->assetID = payload_res->assetID;

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
                GameObject* go = App->scene->CreateGO(payload_res->assetName.c_str(), ListGO[0]);

                for (int ind = 0; ind < payload_model->meshDatas.size(); ind++) {
                    GameObject* goChild = App->scene->CreateGO(payload_res->assetName.c_str(), go);
                    goChild->CreateComp(Component::Types::MESH);
                    goChild->GOmesh->meshData = payload_model->meshDatas[ind];
                    goChild->GOmesh->meshData->LoadBuffers();
                    goChild->GOmesh->meshData->assetID = payload_res->assetID;
                    goChild->GOmesh->meshData->indexInModel = ind;

                    goChild->CreateComp(Component::Types::MATERIAL);

                    if (goChild->GOmesh->meshData->materialAttachedID != 0) {

                        char* fileBuffer = nullptr;
                        std::string libName = std::to_string(goChild->GOmesh->meshData->materialAttachedID) + ".chad";

                        uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                        for (int rInd = 0; rInd < App->scene->resourceList.size(); rInd++) {
                            if (App->scene->resourceList[rInd]->assetID == goChild->GOmesh->meshData->materialAttachedID) {

                                MaterialData* new_material_data = new MaterialData(App->scene->resourceList[rInd]->assetPath.c_str());
                                MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

                                goChild->GOmat->materialData = new_material_data;
                                goChild->GOmat->materialData->assetID = goChild->GOmesh->meshData->materialAttachedID;
                            }
                        }

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
    ControlVehicle();
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
        if (i > 1) {
            saveGoList.push_back(App->scene->ListGO[i]);
        }
    }

    int gameObjects_num = saveGoList.size();

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

        json_object_set_number(json_object(scene_settings), "gameObjects_num", gameObjects_num);

        for (int i = 0; i < saveGoList.size(); i++) {
            
                std::string goName = "Gameobject [" + std::to_string(i) + "] Name";
                json_object_set_string(json_object(scene_settings), goName.c_str(), saveGoList[i]->name.c_str());
                std::string goUid = "Gameobject [" + std::to_string(i) + "] UID";
                json_object_set_number(json_object(scene_settings), goUid.c_str(), saveGoList[i]->uid);
                std::string goParent = "Gameobject [" + std::to_string(i) + "] Parent";
                json_object_set_number(json_object(scene_settings), goParent.c_str(), saveGoList[i]->parent->uid);

                std::string goComp;
                goComp = "Gameobject [" + std::to_string(i) + "] Transform";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
                goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
                goComp = "Gameobject [" + std::to_string(i) + "] Material";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
                goComp = "Gameobject [" + std::to_string(i) + "] Camera";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
                goComp = "Gameobject [" + std::to_string(i) + "] Light";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
                goComp = "Gameobject [" + std::to_string(i) + "] Physics";
                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);

                for (int j = 0; j < saveGoList[i]->components.size(); j++) {
                   
                    switch (saveGoList[i]->components[j]->type) {
                        case Component::Types::TRANSFORM:
                            {

                                goComp = "Gameobject [" + std::to_string(i) + "] Transform";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

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

                                goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);


                                goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset ID";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetID);


                                goComp = "Gameobject [" + std::to_string(i) + "] Mesh Index In Model";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->indexInModel);


                                /*for (int n = 0; n < App->scene->resourceList.size(); n++)
                                {
                                    if (App->scene->resourceList[n]->assetID == saveGoList[i]->GOmesh->meshData->assetID) {
                                       
                                    }
                                }*/
                               // goComp = "Gameobject [" + std::to_string(i) + "] Mesh UID";
                                /*for (int k = 0; k < App->scene->resourceList.size(); k++) {
                                    if (App->scene->resourceList[k]->assetID == saveGoList[i]->GOmesh->meshData->assetID) {
                                        App->scene->LoadSpecific(App->scene->resourceList[k]->assetID);
                                    }

                                }*/
                               // json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetID);

                               // goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset Name";
                               // json_object_set_string(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetName.c_str());

                            }
                        break;
                        case Component::Types::MATERIAL:
                            {
                                if (saveGoList[i]->GOmat->materialData != nullptr) {
                                    goComp = "Gameobject [" + std::to_string(i) + "] Material";
                                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

                                    goComp = "Gameobject [" + std::to_string(i) + "] Material Asset ID";
                                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmat->materialData->assetID);

                                }
                            }
                        break;
                        case Component::Types::CAMERA:
                            {
                                goComp = "Gameobject [" + std::to_string(i) + "] Camera";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);
                            }
                        break;
                        case Component::Types::LIGHT:
                            {
                                goComp = "Gameobject [" + std::to_string(i) + "] Light";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);
                            }
                        break;
                        case Component::Types::PHYSICS:
                            {
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Shape Selected";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), (int)saveGoList[i]->GOphys->shapeSelected);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Is Static";
                                json_object_set_boolean(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->isStatic);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.z);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.z);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl X";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Y";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.y);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Z";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.z);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Sphere Radius";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->sphereRadius);

                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Radius";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->cylRadiusHeight.x);
                                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Height";
                                json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->cylRadiusHeight.y);

                            }
                        break;
                   
                    };
                    
                }
            
        }
        

        json_serialize_to_file(scene_settings, "Settings/scene_at_play.json");
    }

    json_serialize_to_file_pretty(scene_settings, "Settings/scene_at_play.json");

    json_value_free(schema);
    json_value_free(scene_settings);

    return true;
}

bool ModuleScene::LoadSceneAtPlay() {

    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("Settings/scene_at_play.json");
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

    int gameObjects_num = 0;
    std::vector <GameObject*> saveGoList;

    float3 parentPos;

    if (scene_settings == NULL || json_validate(schema, scene_settings) != JSONSuccess) 
    {
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
        show_console_window = json_object_get_boolean(json_object(scene_settings), "show_console_window");
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

        gameObjects_num = json_object_get_number(json_object(scene_settings), "gameObjects_num");


        //Restore GameObjects
        for (int i = 0; i < gameObjects_num; i++)
        {
            std::string nameField = "Gameobject [" + std::to_string(i) + "] Name";
            const char* goName = json_object_get_string(json_object(scene_settings), nameField.c_str());

            GameObject* go = App->scene->CreateGO(goName, App->scene->ListGO[0]);

            std::string uidField = "Gameobject [" + std::to_string(i) + "] UID";
            int goUid = json_object_get_number(json_object(scene_settings), uidField.c_str());

            go->uid = goUid;

            saveGoList.push_back(go);

        }

        //Set parents
        for (int i = 0; i < saveGoList.size(); i++)
        {
            std::string parentField = "Gameobject [" + std::to_string(i) + "] Parent";
            int goParent = json_object_get_number(json_object(scene_settings), parentField.c_str());

            if (goParent != 0) {

                for (int j = 0; j < saveGoList.size(); j++)
                {
                    if (saveGoList[j]->uid == goParent)
                    {
                        saveGoList[j]->children.push_back(saveGoList[i]);
                        saveGoList[i]->parent = saveGoList[j];
                    }
                }
            }
        }

        for (int i = 0; i < saveGoList.size(); i++)
        {

            GameObject* go = saveGoList[i];

            std::string goComp;
            bool hasComp = false;

            goComp = "Gameobject [" + std::to_string(i) + "] Transform";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                float3 goPos;
                float3 goRot;
                float3 goScl;

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos X";
                goPos[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Y";
                goPos[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Z";
                goPos[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot X";
                goRot[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Y";
                goRot[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Z";
                goRot[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl X";
                goScl[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Y";
                goScl[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Z";
                goScl[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                //goComp = "Gameobject [" + std::to_string(i) + "] Parent";
                //uint parentID = json_object_get_number(json_object(scene_settings), goComp.c_str());


                //if (parentID == 0) {
                    go->GOtrans->SetPos(goPos);
                    //parentPos = goPos;
                    go->GOtrans->SetRot(goRot);
                    go->GOtrans->SetScale(goScl);
               // }

              
                

                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset ID";
                int assetID = json_object_get_number(json_object(scene_settings), goComp.c_str());

                
                for (int j = 0; j < App->scene->resourceList.size(); j++) 
                {
                    if (App->scene->resourceList[j]->assetID == assetID) {
                        Resource* payload_res = App->scene->resourceList[j];

                        ModelData* payload_model = (ModelData*)payload_res;

                        if (payload_model->meshDatas.size() == 1) {

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
                        }
                        else {

                            goComp = "Gameobject [" + std::to_string(i) + "] Mesh Index In Model";
                            int indexInModel = json_object_get_number(json_object(scene_settings), goComp.c_str());


                            char* fileBufferMesh = nullptr;
                            std::string libName = std::to_string(payload_res->assetID) + ".chad";
                            uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

                            ModelImporter::Load(fileBufferMesh, payload_model);
                                 
                            go->CreateComp(Component::Types::MESH);
                            go->GOmesh->meshData = payload_model->meshDatas[indexInModel];
                            go->GOmesh->meshData->LoadBuffers();
                            go->GOmesh->meshData->assetID = payload_res->assetID;
                            go->GOmesh->meshData->indexInModel = indexInModel;

                            go->CreateComp(Component::Types::MATERIAL);

                        }
                    }
                }
               // goComp = "Gameobject [" + std::to_string(i) + "] Mesh UID";
               // int assetID = json_object_get_number(json_object(scene_settings), goComp.c_str());
              
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Material";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                goComp = "Gameobject [" + std::to_string(i) + "] Material Asset ID";
                int assetID = json_object_get_number(json_object(scene_settings), goComp.c_str());

                for (int j = 0; j < App->scene->resourceList.size(); j++)
                {
                    if (App->scene->resourceList[j]->assetID == assetID) {
                        Resource* payload_res = App->scene->resourceList[j];

                        go->CreateComp(Component::Types::MATERIAL);

                        char* fileBuffer = nullptr;
                        std::string libName = std::to_string(payload_res->assetID) + ".chad";

                        uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                        MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
                        MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

                        go->GOmat->materialData = new_material_data;
                        go->GOmat->materialData->assetID = assetID;
                    }
                }

                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Camera";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {
                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Light";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {
                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Physics";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                go->CreateComp(Component::Types::PHYSICS);
                go->GOphys->phys = App->physics;

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Shape Selected";
                go->GOphys->shapeSelected = (CPhysics::ColliderShape)json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Is Static";
                go->GOphys->isStatic = json_object_get_boolean(json_object(scene_settings), goComp.c_str());

                float3 newColPos;
                float3 newColRot;
                float3 newColScl;

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos X";
                newColPos.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Y";
                newColPos.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Z";
                newColPos.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot X";
                newColRot.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Y";
                newColRot.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Z";
                newColRot.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl X";
                newColScl.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Y";
                newColScl.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Z";
                newColScl.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                go->GOphys->colPos = {newColPos.x, newColPos.y, newColPos.z};
                go->GOphys->colRot = {newColRot.x, newColRot.y, newColRot.z};
                go->GOphys->colScl = {newColScl.x, newColScl.y, newColScl.z};

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Sphere Radius";
                go->GOphys->sphereRadius = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Radius";
                go->GOphys->cylRadiusHeight.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Height";
                go->GOphys->cylRadiusHeight.y = json_object_get_number(json_object(scene_settings), goComp.c_str());

                go->GOphys->CreateCollider();
                go->GOphys->CallUpdateShape();

                hasComp = false;
            }
        }

        

        //Add them to GameObjects List
        //for (int i = 0; i < saveGoList.size(); i++) {
           
           // App->scene->AddGOList(saveGoList[i]);
       // }

         
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

    //App->scene->ListGO[2]->GOtrans->SetPos(parentPos);
    //App->scene->ListGO[2]->GOtrans->SetRot(goRot);
    //App->scene->ListGO[2]->GOtrans->SetScale(goScl);

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

    std::vector <GameObject*> saveGoList;

    for (int i = 0; i < App->scene->ListGO.size(); i++) {
        if (i > 1) {
            saveGoList.push_back(App->scene->ListGO[i]);
        }
    }

    int gameObjects_num = saveGoList.size();

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

        json_object_set_number(json_object(scene_settings), "gameObjects_num", gameObjects_num);

        for (int i = 0; i < saveGoList.size(); i++) {

            std::string goName = "Gameobject [" + std::to_string(i) + "] Name";
            json_object_set_string(json_object(scene_settings), goName.c_str(), saveGoList[i]->name.c_str());
            std::string goUid = "Gameobject [" + std::to_string(i) + "] UID";
            json_object_set_number(json_object(scene_settings), goUid.c_str(), saveGoList[i]->uid);
            std::string goParent = "Gameobject [" + std::to_string(i) + "] Parent";
            json_object_set_number(json_object(scene_settings), goParent.c_str(), saveGoList[i]->parent->uid);

            std::string goComp;
            goComp = "Gameobject [" + std::to_string(i) + "] Transform";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
            goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
            goComp = "Gameobject [" + std::to_string(i) + "] Material";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
            goComp = "Gameobject [" + std::to_string(i) + "] Camera";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
            goComp = "Gameobject [" + std::to_string(i) + "] Light";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);
            goComp = "Gameobject [" + std::to_string(i) + "] Physics";
            json_object_set_boolean(json_object(scene_settings), goComp.c_str(), false);

            for (int j = 0; j < saveGoList[i]->components.size(); j++) {

                switch (saveGoList[i]->components[j]->type) {
                case Component::Types::TRANSFORM:
                {

                    goComp = "Gameobject [" + std::to_string(i) + "] Transform";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

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

                    goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

                    int modelCount = 0;
                    goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset File Name";
                    for (int k = 0; k < App->scene->resourceList.size(); k++) {
                        if (App->scene->resourceList[k]->resourceType == Resource::Types::MODEL) {
                            for (int l = 0; l < ((ModelData*)App->scene->resourceList[k])->meshDatas.size(); l++) {
                                if (((ModelData*)App->scene->resourceList[k])->meshDatas[l] == saveGoList[i]->GOmesh->meshData) {

                                    json_object_set_string(json_object(scene_settings), goComp.c_str(), App->scene->resourceList[k]->fileName.c_str());
                                    modelCount++;

                                }
                            }
                        }
                    }

                    if (modelCount == 0) {
                        json_object_set_string(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->fileName.c_str());
                    }
                   

                    goComp = "Gameobject [" + std::to_string(i) + "] Mesh Index In Model";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->indexInModel);


                    /*for (int n = 0; n < App->scene->resourceList.size(); n++)
                    {
                        if (App->scene->resourceList[n]->assetID == saveGoList[i]->GOmesh->meshData->assetID) {

                        }
                    }*/
                    // goComp = "Gameobject [" + std::to_string(i) + "] Mesh UID";
                     /*for (int k = 0; k < App->scene->resourceList.size(); k++) {
                         if (App->scene->resourceList[k]->assetID == saveGoList[i]->GOmesh->meshData->assetID) {
                             App->scene->LoadSpecific(App->scene->resourceList[k]->assetID);
                         }

                     }*/
                     // json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetID);

                     // goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset Name";
                     // json_object_set_string(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmesh->meshData->assetName.c_str());

                }
                break;
                case Component::Types::MATERIAL:
                {
                    if (saveGoList[i]->GOmat->materialData != nullptr) {
                        goComp = "Gameobject [" + std::to_string(i) + "] Material";
                        json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

                        goComp = "Gameobject [" + std::to_string(i) + "] Material Asset File Name";
                        json_object_set_string(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOmat->materialData->fileName.c_str());

                    }
                }
                break;
                case Component::Types::CAMERA:
                {
                    goComp = "Gameobject [" + std::to_string(i) + "] Camera";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);
                }
                break;
                case Component::Types::LIGHT:
                {
                    goComp = "Gameobject [" + std::to_string(i) + "] Light";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);
                }
                break;
                case Component::Types::PHYSICS:
                {
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), true);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Shape Selected";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), (int)saveGoList[i]->GOphys->shapeSelected);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Is Static";
                    json_object_set_boolean(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->isStatic);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos X";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.x);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Y";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.y);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Z";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colPos.z);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot X";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.x);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Y";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.y);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Z";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colRot.z);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl X";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.x);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Y";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.y);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Z";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->colScl.z);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Sphere Radius";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->sphereRadius);

                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Radius";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->cylRadiusHeight.x);
                    goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Height";
                    json_object_set_number(json_object(scene_settings), goComp.c_str(), saveGoList[i]->GOphys->cylRadiusHeight.y);

                }
                break;

                };

            }

        }

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

    int gameObjects_num = 0;
    std::vector <GameObject*> saveGoList;

    float3 parentPos;

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
        
        gameObjects_num = json_object_get_number(json_object(scene_settings), "gameObjects_num");


        //Restore GameObjects
        for (int i = 0; i < gameObjects_num; i++)
        {
            std::string nameField = "Gameobject [" + std::to_string(i) + "] Name";
            const char* goName = json_object_get_string(json_object(scene_settings), nameField.c_str());

            GameObject* go = App->scene->CreateGO(goName, App->scene->ListGO[0]);

            std::string uidField = "Gameobject [" + std::to_string(i) + "] UID";
            int goUid = json_object_get_number(json_object(scene_settings), uidField.c_str());

            go->uid = goUid;

            saveGoList.push_back(go);

        }

        //Set parents
        for (int i = 0; i < saveGoList.size(); i++)
        {
            std::string parentField = "Gameobject [" + std::to_string(i) + "] Parent";
            int goParent = json_object_get_number(json_object(scene_settings), parentField.c_str());

            if (goParent != 0) {

                for (int j = 0; j < saveGoList.size(); j++)
                {
                    if (saveGoList[j]->uid == goParent)
                    {
                        saveGoList[j]->children.push_back(saveGoList[i]);
                        saveGoList[i]->parent = saveGoList[j];
                    }
                }
            }
        }

        for (int i = 0; i < saveGoList.size(); i++)
        {

            GameObject* go = saveGoList[i];

            std::string goComp;
            bool hasComp = false;

            goComp = "Gameobject [" + std::to_string(i) + "] Transform";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                float3 goPos;
                float3 goRot;
                float3 goScl;

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos X";
                goPos[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Y";
                goPos[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Pos Z";
                goPos[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot X";
                goRot[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Y";
                goRot[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Rot Z";
                goRot[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl X";
                goScl[0] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Y";
                goScl[1] = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Transform Scl Z";
                goScl[2] = json_object_get_number(json_object(scene_settings), goComp.c_str());

                //goComp = "Gameobject [" + std::to_string(i) + "] Parent";
                //uint parentID = json_object_get_number(json_object(scene_settings), goComp.c_str());


                //if (parentID == 0) {
                go->GOtrans->SetPos(goPos);
                //parentPos = goPos;
                go->GOtrans->SetRot(goRot);
                go->GOtrans->SetScale(goScl);
                // }




                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Mesh";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                goComp = "Gameobject [" + std::to_string(i) + "] Mesh Asset File Name";
                std::string assetFileName = json_object_get_string(json_object(scene_settings), goComp.c_str());


                for (int j = 0; j < App->scene->resourceList.size(); j++)
                {
                    if (App->scene->resourceList[j]->fileName == assetFileName) {
                        Resource* payload_res = App->scene->resourceList[j];

                        ModelData* payload_model = (ModelData*)payload_res;

                        if (payload_model->meshDatas.size() == 1) {

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
                        }
                        else {

                            goComp = "Gameobject [" + std::to_string(i) + "] Mesh Index In Model";
                            int indexInModel = json_object_get_number(json_object(scene_settings), goComp.c_str());


                            char* fileBufferMesh = nullptr;
                            std::string libName = std::to_string(payload_res->assetID) + ".chad";
                            uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBufferMesh);

                            ModelImporter::Load(fileBufferMesh, payload_model);

                            go->CreateComp(Component::Types::MESH);
                            go->GOmesh->meshData = payload_model->meshDatas[indexInModel];
                            go->GOmesh->meshData->LoadBuffers();
                            go->GOmesh->meshData->assetID = payload_res->assetID;
                            go->GOmesh->meshData->indexInModel = indexInModel;

                            go->CreateComp(Component::Types::MATERIAL);

                        }
                    }
                }
                // goComp = "Gameobject [" + std::to_string(i) + "] Mesh UID";
                // int assetID = json_object_get_number(json_object(scene_settings), goComp.c_str());

            }

            goComp = "Gameobject [" + std::to_string(i) + "] Material";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                goComp = "Gameobject [" + std::to_string(i) + "] Material Asset File Name";
                std::string assetFileName = json_object_get_string(json_object(scene_settings), goComp.c_str());

                for (int j = 0; j < App->scene->resourceList.size(); j++)
                {
                    if (App->scene->resourceList[j]->fileName == assetFileName) {
                        Resource* payload_res = App->scene->resourceList[j];

                        go->CreateComp(Component::Types::MATERIAL);

                        char* fileBuffer = nullptr;
                        std::string libName = std::to_string(payload_res->assetID) + ".chad";

                        uint bufferSize = App->fileSystem->FileToBuffer(libName.c_str(), &fileBuffer);
                        MaterialData* new_material_data = new MaterialData(payload_res->assetPath.c_str());
                        MaterialImporter::Load(fileBuffer, new_material_data, bufferSize);

                        go->GOmat->materialData = new_material_data;
                        go->GOmat->materialData->assetID = App->scene->resourceList[j]->assetID;
                    }
                }

                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Camera";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {
                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Light";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {
                hasComp = false;
            }

            goComp = "Gameobject [" + std::to_string(i) + "] Physics";
            hasComp = json_object_get_boolean(json_object(scene_settings), goComp.c_str());
            if (hasComp == true) {

                go->CreateComp(Component::Types::PHYSICS);
                go->GOphys->phys = App->physics;

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Shape Selected";
                go->GOphys->shapeSelected = (CPhysics::ColliderShape)json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Is Static";
                go->GOphys->isStatic = json_object_get_boolean(json_object(scene_settings), goComp.c_str());

                float3 newColPos;
                float3 newColRot;
                float3 newColScl;

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos X";
                newColPos.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Y";
                newColPos.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Pos Z";
                newColPos.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot X";
                newColRot.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Y";
                newColRot.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Rot Z";
                newColRot.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl X";
                newColScl.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Y";
                newColScl.y = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Col Scl Z";
                newColScl.z = json_object_get_number(json_object(scene_settings), goComp.c_str());

                go->GOphys->colPos = { newColPos.x, newColPos.y, newColPos.z };
                go->GOphys->colRot = { newColRot.x, newColRot.y, newColRot.z };
                go->GOphys->colScl = { newColScl.x, newColScl.y, newColScl.z };

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Sphere Radius";
                go->GOphys->sphereRadius = json_object_get_number(json_object(scene_settings), goComp.c_str());

                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Radius";
                go->GOphys->cylRadiusHeight.x = json_object_get_number(json_object(scene_settings), goComp.c_str());
                goComp = "Gameobject [" + std::to_string(i) + "] Physics Cyl Height";
                go->GOphys->cylRadiusHeight.y = json_object_get_number(json_object(scene_settings), goComp.c_str());

                go->GOphys->CreateCollider();
                go->GOphys->CallUpdateShape();

                hasComp = false;
            }
        }

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

