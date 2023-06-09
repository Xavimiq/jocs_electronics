#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "Entity.h"
#include "Scene.h"
#include "Stage.h"


#include <cmath>

class Scene;
class Entity;
//some globals
Mesh* mesh = NULL;
Mesh* mesh2 = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Shader* shader2 = NULL;
Animation* anim = NULL;
float angle = 0;
float position_z = 5;
float mouse_speed = 100.0f;
FBO* fbo = NULL;
bool freecam = true;
Game* Game::instance = NULL;
//EntityMesh* e_mesh = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(1, 1, 1));
float delta_yaw;
float delta_pitch;

Stage* gameplay = new Stage();

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	
	this->activeStage = gameplay;
	
	
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/untitled.obj");
	mesh2 = Mesh::Get("data/box.ASE");
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");
	shader2 = Shader::Get("data/shaders/basic.vs", "data/shaders/fong.fs");
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}


//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();
	
	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	this->activeStage->render();
   
	//Entity* entity = new Entity(NULL);
	EntityMesh* e_mesh = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(1,1,1));
	//EntityMesh* e_mesh2 = new EntityMesh(NULL, "ent", mesh2, texture, shader2, Vector3(100, 1, 1));
	
	for (int i = 0; i < 5; i++)
	{
		for  (int j = 0; j < 5; j++)
		{
			EntityMesh* e_mesh_child = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(i*50, j*50, 1));
			e_mesh->addChild(e_mesh_child);
		}
	}
	
	
	e_mesh->model.translate(0, 0, position_z);

	//rotate;
	
	//this->activeScene->render();
	//create model matrix for cube
	Matrix44 m;

	//

	//
	e_mesh->render();
	
	if(shader)
	{
		/*
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", time);


		shader->setUniform("u_camera_pos", camera->eye);
		shader->setUniform("u_light_pos", Vector3(400, 0, 0));

		//do the draw call
		mesh->render( GL_TRIANGLES );
		//mesh2->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
		*/

	}
	

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);

	
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	//e_mesh->update(seconds_elapsed);
	//this->activeScene->update(seconds_elapsed);
	//this->activeStage->update(seconds_elapsed);
	//example
	angle += (float)seconds_elapsed * 10.0f;
	position_z += (float)seconds_elapsed * 10.0f;


	//Movment of the camera
	Input::centerMouse();

	delta_yaw += Input::mouse_delta.y * 0.005f;
	delta_pitch += Input::mouse_delta.x * 0.005f;

	delta_yaw = fmod(delta_yaw, 2 * M_PI);
	delta_pitch = fmod(delta_pitch, 2 * M_PI);

	delta_yaw = clamp(delta_yaw, -M_PI * 0.25, M_PI * 0.25);

	Matrix44 mPitch, mYaw;
	mPitch.setRotation(delta_pitch, Vector3(0, -1, 0));
	mYaw.setRotation(delta_yaw, camera->getLocalVector(Vector3(-1, 0, 0)));

	camera->fromRotationMatrix(mPitch * mYaw);

	//mouse input to rotate the cam
	/*
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}
	*/
	//async input to move the camera around
	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	
	
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDL_SCANCODE_P: 
			this->activeStage = gameplay;
			this->activeStage->render();
			break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

