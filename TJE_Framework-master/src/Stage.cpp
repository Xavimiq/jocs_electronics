#include "Stage.h"


class Entity;
class Shader;
class Texture;
class Mesh;
class camera;
class Scene;
class Game;
class World;
class Input;



Stage::Stage()
{
	//this->stageType = eStageType::BASE;	//new_camera = new Camera();

}



void Stage::render()
{
}

void Stage::update(float seconds_elapsed)
{

	//if (this->scene)y
		//this->scene->update(seconds_elapsed);
	//my_player->update(seconds_elapsed);
}



PlayStage::PlayStage() : Stage() {
	
	Texture* texture;
	texture = new Texture();
	texture->load("data/texture.tga");
	
	

	// example of loading Mesh from Mesh Manager
	Mesh* mesh;
	mesh = Mesh::Get("data/untitled.obj");

	// example of shader loading using the shaders manager
	Shader* shader;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");


	e_mesh = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(1, 1, 1));
	

	World* my_world = World::get_instance();

	//my_world = new World();
	my_camera = my_world->camera;
	//my_camera->eye = Vector3(my_player->position.x, 25, my_player->position.z);
	//this->root->addChild(e_mesh);
}

void PlayStage::render()
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	my_camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//e_mesh->render();
	World::get_instance()->render();




	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	//SDL_GL_SwapWindow(this->window);

}


void PlayStage::update(float seconds_elapsed)
{
	
	World::get_instance()->update(seconds_elapsed);

	//my_camera->fromRotationMatrix(mPitch * mYaw);
	
	
	/*
	//float speed = seconds_elapsed * 100.0f; //the speed is defined by the seconds_elapsed so it goes constant
	//Movment of the camera
	Input::centerMouse();

	delta_yaw += Input::mouse_delta.y * 0.005f;
	delta_pitch += Input::mouse_delta.x * 0.005f;

	delta_yaw = fmod(delta_yaw, 2 * M_PI);
	delta_pitch = fmod(delta_pitch, 2 * M_PI);

	delta_yaw = clamp(delta_yaw, -M_PI * 0.25, M_PI * 0.25);

	Matrix44 mPitch, mYaw;
	mPitch.setRotation(delta_pitch, Vector3(0, -1, 0));
	mYaw.setRotation(delta_yaw, my_camera->getLocalVector(Vector3(-1, 0, 0)));
	Matrix44 camRot = mPitch * mYaw;

	my_camera->fromRotationMatrix(camRot);
	
	my_camera->eye.y = 25;
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) my_camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) my_camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) my_camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) my_camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_LCTRL))
	{
		my_camera->eye.y = 20;
		speed *= 0.5;
	}
	*/
	
	

	//my_camera->eye = position;

	Stage::update(seconds_elapsed);
}


MenuStage::MenuStage() : Stage() {

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	
	root = new Entity();
	Texture* texture;
	texture = new Texture();
	texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	Mesh* mesh;
	mesh = Mesh::Get("data/box.ASE");
	//mesh2 = Mesh::Get("data/untitled.ASE");
	//my_player = new EntityPlayer(Vector3(100, 100, 100));
	
	my_camera = new Camera();
	my_camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	my_camera->setPerspective(70.f, width / (float)height, 0.1f, 10000.f);

	// example of shader loading using the shaders manager
	Shader* shader;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			EntityMesh* e_mesh_child = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(i * 200, j * 200, 1));
			this->root->addChild(e_mesh_child);
		}
	}
		
}

void MenuStage::render()
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	my_camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//root->rotate(ang, Vector3(0, 1.0f, 0));
	this->root->render();

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	//SDL_GL_SwapWindow(this->window);
	
	//my_camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));
}

void MenuStage::update(float seconds_elapsed)
{
	
	ang *= seconds_elapsed * 5.0f;
	Stage::update(seconds_elapsed);

	// Variable para controlar la rotación automática
	float auto_rotation_speed = -0.0125f;

	// ...

	// Movimiento de la cámara
	Input::centerMouse();

	delta_yaw += auto_rotation_speed;  // Aumentar el delta_yaw automáticamente
	delta_pitch += auto_rotation_speed;

	delta_yaw = fmod(delta_yaw, 2 * M_PI);
	delta_pitch = fmod(delta_pitch, 2 * M_PI);

	delta_yaw = clamp(delta_yaw, -M_PI * 0.25, M_PI * 0.25);

	Matrix44 mPitch, mYaw;
	mPitch.setRotation(delta_pitch, Vector3(0, -1, 0));
	mYaw.setRotation(delta_yaw, my_camera->getLocalVector(Vector3(-1, 0, 0)));
	Matrix44 camRot = mPitch * mYaw;



	my_camera->lookAt(Vector3() - camRot.frontVector() * 2000.0f, Vector3(), Vector3(0.0f, 1.0f, 0.0f));

	

	//my_camera->lookAt(eye, center, Vector3(0.f, 1.f, 0.f));

	//		float eye_x = 100 + 100.0f * cos(ang);
	//		float eye_y = 100 + 100.0f;
	//		float eye_z = 100 + 100.0f * sin(ang);

	//		eye = Vector3(eye_x, eye_y, eye_z);
	//		center = Vector3(100.f, 100.f, 100.f);
}