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
#include "World.h"
#include "Audio.h"

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



float mouse_speed = 100.0f;
FBO* fbo = NULL;
bool freecam = true;
Game* Game::instance = NULL;

bool Game::menuMusic = false;
bool Game::gameMusic = false;
bool Game::winMusic = false;
bool Game::loseMusic = false;



Game::Game(int window_width, int window_height, SDL_Window* window)
{
	//Audio
	Audio::Init();
	
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

	
	menu = new MenuStage();
	gameplay = new PlayStage();
	win = new WinStage();
	lost = new LostStage();
	current_stage = Menu;

	//SET MENU 
	this->current = menu;	
	camera = this->current->my_camera;
	


	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/untitled.obj");
	//mesh2 = Mesh::Get("data/cube.obj");

	
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");
	//shader2 = Shader::Get("data/shaders/basic.vs", "data/shaders/fong.fs");

}


//what to do when the image has to be draw
void Game::render(void)
{
	//if (current_stage = Play) { current = gameplay; this->camera = this->current->my_camera;}
	if (current_stage == Menu) { 
		current = menu; 
		this->camera = this->current->my_camera; 
		mouse_locked = false;
		SDL_ShowCursor(true);

		if (!menuMusic) {
			menuMusic = true;
			World::audios[6]->Play();
		}
	}
	if (current_stage == Play) 
	{ 
		World::audios[6]->Stop();
		current = gameplay; 
		this->camera = this->current->my_camera; 
		mouse_locked = true;
		SDL_ShowCursor(false);

		if (!gameMusic) {
			gameMusic = true;
			World::audios[7]->Play();
		}
	}
	if (current_stage == Exit) { must_exit = true; }
	if (current_stage == Win) 
	{ 
		World::audios[7]->Stop();
		current = win;
		this->camera = this->current->my_camera;

		if (!winMusic) {
			winMusic = true;
			World::audios[8]->Play();
		}
	}
	if (current_stage == Lost) 
	{ 
		World::audios[7]->Stop();
		current = lost;
		if (!loseMusic) {
			loseMusic = true;
			World::audios[13]->Play();
		}
	}
	
	//Draw the floor grid
	//drawGrid();
	
	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	drawText(2, 20, "my_time", Vector3(1, 1, 1), 2);
	this->current->render();
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);

}

void Game::update(double seconds_elapsed)
{
	//float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	
		
	this->current->update(seconds_elapsed);

}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDLK_F2:
			this->current = this->gameplay;
			this->camera = this->current->my_camera;
			break;
		case SDLK_F3:
			freecam = !freecam;
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




