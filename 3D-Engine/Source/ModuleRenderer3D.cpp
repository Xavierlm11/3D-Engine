#include "Globals.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Source/External/Glew/libx86/glew32.lib")

#include "Primitive.h"


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	context = nullptr;
	mode = RenderMode::NORMAL;
	buffersLoaded = false;
	hasLoadedMesh = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	//Glew library
	GLenum err = glewInit();
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	//Current hardware and driver capabilities
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	if(ret == true)
	{

		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		


		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);

		//glEnable(GL_TEXTURE_2D);

		{
			///// <summary>
					///// //////////////
					///// </summary>
					///// <returns></returns>
					//
					////App->scene->LoadScene();

					//int w;
					//int h;

					//SDL_GetWindowSize(App->window->window, &w, &h);

					//OnResize(0, 0, App->window->winWidth, App->window->winHeight);
		}
		//int w;
		//int h;

		//SDL_GetWindowSize(App->window->window, &w, &h);
		////App->renderer3D->OnResize(0, 0, w, h);

		//glViewport(0, 0, w, h);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//ProjectionMatrix = perspective(60.0f, (float)w / (float)h, 0.125f, 512.0f);
		//glLoadMatrixf(&ProjectionMatrix);

		//glMatrixMode(GL_MODELVIEW);

		OnResize(0,0, App->window->winWidth, App->window->winHeight );

	}

	/*///[VERTICES]
	static const float vertices[] =
	{ 0.f, 0.f, 0.f,
		5.f, 0.f, 0.f,
		0.f, 5.f, 0.f,

		5.f, 0.f, 0.f,
		5.f, 5.f, 0.f,
		0.f, 5.f, 0.f };

	num_vertices = 6;
	indices =
	{
		0,
		1,
		2,

		1,
		3,
		2
	};


	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* indices.size(), &indices[0], GL_STATIC_DRAW);*/

	num_vertices = 8;

	my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_vertices * 3, vertices, GL_STATIC_DRAW);

	num_indices = 36;

	my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* num_indices, indices, GL_STATIC_DRAW);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Color c = App->camera->background;
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)App->camera->GetViewMatrixOpenGL());

	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}
update_status ModuleRenderer3D::Update(float dt)
{
	//DrawDirectCube();
	//DrawArrayCube();
	//DrawArrayIndexCube();
	
	/*glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 5.f, 0.f);
	glEnd();
	glLineWidth(1.0f);*/

	

	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::LoadModelBuffers(MeshData *mesh) {



	glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, &mesh->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, &mesh->indices[0], GL_STATIC_DRAW);

	hasLoadedMesh = true;
	LOG("Mesh Loaded! Mesh indices: %i. Mesh vertices: %i", mesh->num_indices, mesh->num_vertices);
}

void ModuleRenderer3D::DrawMesh(MeshData* mesh) {
	

	if (hasLoadedMesh == true) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		LOG("Mesh Loaded! Num indices: %i. Num vertices: %i. ID Indices: %i. ID Vertices: %i.", mesh->num_indices, mesh->num_vertices, mesh->id_indices, mesh->id_vertices);
	}

	{
		//if (buffersLoaded == true) {

		//	//glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
		//	//glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		//	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, &mesh.vertices[0], GL_STATIC_DRAW);
		//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//	//glGenBuffers(1, (GLuint*)&(mesh.id_indices));
		//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices * 3, &mesh.indices[0], GL_STATIC_DRAW);
		//	//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//	/*glEnableClientState(GL_VERTEX_ARRAY);
		//	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		//	glVertexPointer(3, GL_FLOAT, 0, NULL);

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);

		//	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	glDisableClientState(GL_VERTEX_ARRAY);*/


		//}

		////glLineWidth(2.0f);
		////glBegin(GL_TRIANGLES);
		////glVertex3f(0.f, 0.f, 0.f);
		////glVertex3f(5.f, 0.f, 0.f);
		////glVertex3f(0.f, 5.f, 0.f);
		////
		////glEnd();
		////glLineWidth(1.0f);



		/////[DRAW]
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glVertexAttribPointer(
		//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		//	3,                  // size
		//	GL_FLOAT,           // type
		//	GL_FALSE,           // normalized?
		//	0,                  // stride
		//	(void*)0            // array buffer offset
		//);
		//// Draw the triangle !
		////glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		//glDrawElements(GL_TRIANGLES,      // mode
		//	indices.size(),    // count
		//	GL_UNSIGNED_INT,   // type
		//	(void*)0);
		//glDisableVertexAttribArray(0);


		///*glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
		//glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);*/

		////glEnableClientState(GL_VERTEX_ARRAY);
		////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		////glDrawElements(GL_TRIANGLES,  num_indices, GL_UNSIGNED_INT, nullptr);
		////glDisableClientState(GL_VERTEX_ARRAY);





		///*glEnableClientState(GL_VERTEX_ARRAY);
		//glBindBuffer(GL_ARRAY_BUFFER, my_id);
		//glVertexPointer(3, GL_FLOAT, 0, NULL);
		//glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		//glDisableClientState(GL_VERTEX_ARRAY);*/

		////glBindBuffer(GL_ARRAY_BUFFER, my_id);
		////glVertexPointer(3, GL_FLOAT, 0, NULL);
		////// … bind and use other buffers
		////glDrawArrays(GL_TRIANGLES, 0, mesh.num_vertices);
		////glDisableClientState(GL_VERTEX_ARRAY);
	}


	/*uint indices[] =
	{
		0,
		1,
		2,

		1,
		3,
		2
	};*/

	///// <summary>
	///// //[CUBEEEEEEEEEEEEE]
	///// </summary>
	///// <param name="mesh"></param>

	//glEnableClientState(GL_VERTEX_ARRAY);
	////glBindBuffer(GL_ARRAY_BUFFER, my_indices);
	//

	////glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	//glDisableClientState(GL_VERTEX_ARRAY);

}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	PrimPlane p(0, 0, 0, 0);
	p.axis = true;

	//if (mode == RenderMode::NORMAL) {
	//	PrimPlane p(0, 0, 0, 0);
	//	p.axis = true;
	//	p.Render();

	//	Draw();
	//}
	//else if (mode == RenderMode::WIREFRAME) {

	//	Draw();
	//}
	//OnResize(0, 0, App->window->winWidth, App->window->winHeight);

	switch(mode)
	{
	case RenderMode::NONE:

		break;
	case RenderMode::NORMAL:
		p.Render();
		DrawMesh(App->scene->modelMesh);
		Draw();
		break;
	case RenderMode::WIREFRAME:
		Draw();
		break;
	}


	App->editor->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::Draw() {

	//int w, h;
	//SDL_GetWindowSize(App->window->window, &w, &h);

	//glViewport(0, 0, w, h);

	////glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//ProjectionMatrix = perspective(60.0f, (float)w / (float)h, 0.125f, 512.0f);
	//glLoadMatrixf(&ProjectionMatrix);

	//glMatrixMode(GL_MODELVIEW);

	///////*App->window->winWidth = width;
	//////App->window->winHeight = height;

	//////glLoadIdentity();
	//////ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	//////glLoadMatrixf(&ProjectionMatrix);*/

}

void ModuleRenderer3D::OnResize(int x, int y, int width, int height)
{
		App->window->winWidth = width;
		App->window->winHeight = height;

		glViewport(x, y, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
		glLoadMatrixf(&ProjectionMatrix);

		glMatrixMode(GL_MODELVIEW);

}


void ModuleRenderer3D::DrawArrayCube()
{
	uint my_id = 0;
	float vertices[] = {
		//Vertices according to faces
		-1.0f,-1.0f,-1.0f, // triángulo 1 : comienza
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triángulo 1 : termina
		1.0f, 1.0f,-1.0f, // triángulo 2 : comienza
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triángulo 2 : termina
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void ModuleRenderer3D::DrawArrayIndexCube()
{
	uint my_id = 0;
	GLfloat vertices2[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
						1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
						1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
					   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
					   -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
						1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 };
	GLubyte indices[] = { 0,1,2, 2,3,0,   // 36 of indices
					 0,3,4, 4,5,0,
					 0,5,6, 6,1,0,
					 1,6,7, 7,2,1,
					 7,4,3, 3,2,7,
					 4,7,6, 6,5,4 };


	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices2, GL_STATIC_DRAW);

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);


	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	//// activate and specify pointer to vertex array
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, vertices);

	//// draw first half, range is 6 - 0 + 1 = 7 vertices used
	//glDrawRangeElements(GL_TRIANGLES, 0, 6, 18, GL_UNSIGNED_BYTE, indices);

	//// draw second half, range is 7 - 1 + 1 = 7 vertices used
	//glDrawRangeElements(GL_TRIANGLES, 1, 7, 18, GL_UNSIGNED_BYTE, indices + 18);

	//// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);


	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … bind and use other buffers
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDisableClientState(GL_VERTEX_ARRAY);
}
void ModuleRenderer3D::DrawDirectCube()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
	//glRotatef(190.f, 1.0f, 1.0f, 0.0f);
}
