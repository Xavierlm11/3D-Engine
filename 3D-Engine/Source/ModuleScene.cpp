#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "parson/parson.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading scene");
	bool ret = true;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::SaveScene() {

   /* JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* user_data = json_parse_file("user_data.json");
    char buf[256];
    const char* name = NULL;
    if (user_data == NULL || json_validate(schema, user_data) != JSONSuccess) {
        puts("Enter your name:");
        scanf("%s", buf);
        user_data = json_value_init_object();
        json_object_set_string(json_object(user_data), "name", buf);
        json_serialize_to_file(user_data, "user_data.json");
    }
    name = json_object_get_string(json_object(user_data), "name");
    printf("Hello, %s.", name);
    json_value_free(schema);
    json_value_free(user_data);*/

    ImVec4 backgroundColor = App->imGui->clear_color;
    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("scene_settings.json");

    if (scene_settings == NULL || json_validate(schema, scene_settings) != JSONSuccess) {
        scene_settings = json_value_init_object();
        json_object_set_number(json_object(scene_settings), "R", backgroundColor.x);
        json_object_set_number(json_object(scene_settings), "G", backgroundColor.y);
        json_object_set_number(json_object(scene_settings), "B", backgroundColor.z);
        json_object_set_number(json_object(scene_settings), "A", backgroundColor.w);
        json_serialize_to_file(scene_settings, "Settings/scene_settings.json");
    }
   

    json_value_free(schema);
    json_value_free(scene_settings);

	return true;
}

bool ModuleScene::LoadScene() {

    JSON_Value* schema = json_parse_string("{\"name\":\"\"}");
    JSON_Value* scene_settings = json_parse_file("Settings/scene_settings.json");
    ImVec4 new_backgroundColor;
    float name = NULL;
    if (scene_settings == NULL || json_validate(schema, scene_settings) != JSONSuccess) {
        LOG("AAAAAAAAAAAAAAVION");
        //scene_settings = json_value_init_object();
        new_backgroundColor.x = json_object_get_number(json_object(scene_settings), "R");
        new_backgroundColor.y = json_object_get_number(json_object(scene_settings), "G");
        new_backgroundColor.z = json_object_get_number(json_object(scene_settings), "B");
        new_backgroundColor.w = json_object_get_number(json_object(scene_settings), "A");
    }
   
    App->imGui->clear_color = new_backgroundColor;

    json_value_free(schema);
    json_value_free(scene_settings);

    return true;
}

