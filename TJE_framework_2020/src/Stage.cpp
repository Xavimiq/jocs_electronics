#include "Stage.h"


class Entity;
class Shader;
class Texture;
class Mesh;
class camera;
class Scene;
class Game;

Stage::Stage()
{
	//this->stageType = eStageType::BASE;
	this->gameInstance = Game::instance;
	//new_camera = new Camera();

}

Stage::Stage(Scene* scene)
{
	//this->stageType = eStageType::BASE;
	this->scene = scene;
	this->gameInstance = Game::instance;
	//new_camera = new Camera();
}

void Stage::render()
{
	this->scene->render();
}

void Stage::update(float seconds_elapsed)
{

	if (this->scene)
		this->scene->update(seconds_elapsed);
}

Scene* Stage::getScene()
{
	return this->scene;
}

PlayStage::PlayStage() : Stage() {
	root = new Entity(NULL);
	Texture* texture;
	texture = new Texture();
	texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	//mesh = Mesh::Get("data/untitled.obj");
	Mesh* mesh2;
	mesh2 = Mesh::Get("data/untitled.ASE");
	// example of shader loading using the shaders manager
	Shader* shader;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");
	//shader2 = Shader::Get("data/shaders/basic.vs", "data/shaders/fong.fs");


	//EntityMesh* e_mesh = new EntityMesh(NULL, "ent", mesh2, texture, shader, Vector3(1, 1, 1));
	//EntityMesh* e_mesh2 = new EntityMesh(NULL, "ent", mesh2, texture, shader2, Vector3(100, 1, 1));


	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			EntityMesh* e_mesh_child = new EntityMesh(NULL, "ent", mesh2, texture, shader, Vector3(i * 50, j * 50, 1));
			root->addChild(e_mesh_child);
		}
	}


	//root->model.translate(0, 0, position_z);
}

void PlayStage::render()
{
	this->scene->render();
}


void PlayStage::update(float seconds_elapsed)
{
	Stage::update(seconds_elapsed);
}