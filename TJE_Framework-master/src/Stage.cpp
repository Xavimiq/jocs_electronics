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
class EntityPlayer;

class EntityGUIelement;


Stage::Stage()
{

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



PlayStage::PlayStage() : Stage() 
{
	//World* my_world = World::get_instance();
	

	my_player = new EntityPlayer(Vector3(-240, 10, 70));
	my_camera = my_player->my_camera;




	///RENDER ENEMY

	Texture* texture_enemy;
	texture_enemy = new Texture();
	texture_enemy->load("data/texture.tga");

	Mesh* mesh_enemy;
	mesh_enemy = Mesh::Get("data/Enemy/enemy.obj");

	Shader* shader_enemy;
	shader_enemy = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs");

	std::vector<Vector3> waypoints1;
	waypoints1.push_back(Vector3(220, 0, -210));
	waypoints1.push_back(Vector3(220, 0, 450));
	waypoints1.push_back(Vector3(-57, 0, 450));

	my_enemy_1 = new EntityEnemy("ene1", Vector3(220, 0, -210), mesh_enemy, shader_enemy, texture_enemy, waypoints1, my_player);
	World::get_instance()->root.addChild(my_enemy_1);
	my_enemy_1->cull = false;

	std::vector<Vector3> waypoints2;
	waypoints2.push_back(Vector3(-220, 0, -290));
	waypoints2.push_back(Vector3(-240, 0, -290));
	waypoints2.push_back(Vector3(-240, 0, -375));
	waypoints2.push_back(Vector3(-54, 0, -375));
	waypoints2.push_back(Vector3(-54, 0, -480));

	my_enemy_2 = new EntityEnemy("ene2", Vector3(-220, 0, -290), mesh_enemy, shader_enemy, texture_enemy, waypoints2, my_player);
	World::get_instance()->root.addChild(my_enemy_2);
	my_enemy_2->cull = false;


	
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
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	World::get_instance()->landscape->render();
	glEnable(GL_DEPTH_TEST);
	World::get_instance()->root.render();//->render();
	my_player->render();
	

	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	//SDL_GL_SwapWindow(this->window);

}


void PlayStage::update(float seconds_elapsed)
{
	my_enemy_1->update(seconds_elapsed);
	my_enemy_2->update(seconds_elapsed);


	my_player->update(seconds_elapsed);
	World::get_instance()->landscape->model.setTranslation(my_player->model.getTranslation().x, 0 , my_player->model.getTranslation().z);
	
	World::get_instance()->update(seconds_elapsed);
	my_player->update(seconds_elapsed);
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

	


	//Stage::update(seconds_elapsed);
}


MenuStage::MenuStage() : Stage() {

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	
	my_camera = new Camera();
	my_camera->lookAt(Vector3(0.f, 20.f, 20.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	my_camera->setPerspective(70.f, width / (float)height, 0.1f, 10000.f);

	
	titleButton = new EntityGUIelement(
		Vector2(width/2, height/6),
		Vector2(500, 100),
		Texture::Get("data/MenuStage/Bottons/Title.png"),
		eButtonId::NoButton,
		"entGUI"
	);
	
	guiElements.addChild(titleButton);

	playButton = new EntityGUIelement(
		Vector2(width / 2, height/6 * 2.7f),
		Vector2(200, 100),
		Texture::Get("data/MenuStage/Bottons/Play.png"),
		eButtonId::playButton,
		"entGUI"
	);

	guiElements.addChild(playButton);

	exitButton = new EntityGUIelement(
		Vector2(width / 2, height/6 * 4.5f),
		Vector2(200, 100),
		Texture::Get("data/MenuStage/Bottons/Exit.png"),
		eButtonId::exitButton,
		"entGUI"
	);

	guiElements.addChild(exitButton);

	//______________________________________
	
	Texture* texture_floor;
	texture_floor = new Texture();
	texture_floor->load("data/floor.png");

	Shader* shader_floor;
	shader_floor = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Mesh* floor = new Mesh();
	floor->createPlane(1000*1000);
	floorplane = new EntityMesh(NULL, "floor", floor, texture_floor, shader_floor, Vector3());

	World::get_instance()->rootMenu.addChild(floorplane);

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
	World::get_instance()->rootMenu.render();

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	guiElements.render();
}

void MenuStage::update(float seconds_elapsed)
{
	
	ang *= seconds_elapsed * 5.0f;
	Stage::update(seconds_elapsed);

	guiElements.update(seconds_elapsed);

	// Variable para controlar la rotación automática
	float auto_rotation_speed = -0.00725f;

	delta_yaw += auto_rotation_speed;  
	delta_pitch += auto_rotation_speed;

	delta_yaw = fmod(delta_yaw, 2 * M_PI);
	delta_pitch = fmod(delta_pitch, 2 * M_PI);

	delta_yaw = clamp(delta_yaw, -M_PI * 0.25, M_PI * 0.25);

	Matrix44 mPitch, mYaw;
	mPitch.setRotation(delta_pitch, Vector3(0, -1, 0));
	mYaw.setRotation(delta_yaw, my_camera->getLocalVector(Vector3(-1, 0, 0)));
	Matrix44 camRot = mPitch * mYaw;

	my_camera->lookAt(Vector3() - camRot.frontVector() * 1000.0f, Vector3(), Vector3(0.0f, 1.0f, 0.0f));
}

WinStage::WinStage() : Stage() {

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;

	//my_camera = new Camera();
	//my_camera->lookAt(Vector3(0.f, 20.f, 20.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	//my_camera->setPerspective(70.f, width / (float)height, 0.1f, 10000.f);


	titleButton = new EntityGUIelement(
		Vector2(width / 2, 100),
		Vector2(500, 100),
		Texture::Get("data/WinStage/win_img.png"),
		eButtonId::NoButton,
		"entGUI"
	);

	guiElements.addChild(titleButton);


	//______________________________________

	//Texture* texture_floor;
	//texture_floor = new Texture();
	//texture_floor->load("data/floor.png");

	//Shader* shader_floor;
	//shader_floor = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//Mesh* floor = new Mesh();
	//floor->createPlane(1000 * 1000);
	//floorplane = new EntityMesh(NULL, "floor", floor, texture_floor, shader_floor, Vector3());

	//World::get_instance()->rootMenu.addChild(floorplane);

}

void WinStage::render()
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	//my_camera->enable();



	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//World::get_instance()->rootMenu.render();

	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	guiElements.render();
}

void WinStage::update(float seconds_elapsed)
{

	//ang *= seconds_elapsed * 5.0f;
	Stage::update(seconds_elapsed);

	guiElements.update(seconds_elapsed);

	// Variable para controlar la rotación automática
	
	//float auto_rotation_speed = -0.00725f;

	//delta_yaw += auto_rotation_speed;
	//delta_pitch += auto_rotation_speed;

	//delta_yaw = fmod(delta_yaw, 2 * M_PI);
	//delta_pitch = fmod(delta_pitch, 2 * M_PI);

	//delta_yaw = clamp(delta_yaw, -M_PI * 0.25, M_PI * 0.25);

	//Matrix44 mPitch, mYaw;
	//mPitch.setRotation(delta_pitch, Vector3(0, -1, 0));
	//mYaw.setRotation(delta_yaw, my_camera->getLocalVector(Vector3(-1, 0, 0)));
	//Matrix44 camRot = mPitch * mYaw;

	//my_camera->lookAt(Vector3() - camRot.frontVector() * 1000.0f, Vector3(), Vector3(0.0f, 1.0f, 0.0f));
}