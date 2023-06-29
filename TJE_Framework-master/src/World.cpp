#include "World.h"
#include "game.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "input.h"


#include <fstream>
#include <map>

World* World::instance = nullptr;
std::vector<Audio*> World::audios;


World::World()
{
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	
	
	landscape_cubemap.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
	landscape_cubemap.diffuse = new Texture();
	landscape_cubemap.diffuse->loadCubemap("landscape",
		{
			"data/cubemap/row-2-column-3.png",
			"data/cubemap/row-2-column-1.png",
			"data/cubemap/row-1-column-2.png",
			"data/cubemap/row-3-column-2.png",
			"data/cubemap/row-2-column-2.png",
			"data/cubemap/row-2-column-4.png",
			
		});
		
	landscape = new EntityMesh(NULL, "sky", Mesh::Get("data/cubemap.ASE"), landscape_cubemap.diffuse, landscape_cubemap.shader, Vector3() );
	
	//root.addChild(landscape);
	

	gui_camera = new Camera();
	gui_camera->view_matrix = Matrix44();
	gui_camera->setOrthographic(0, width, height, 0, -1.0, 1.0);


	

	////----------------------------------------------------------------///

		
	//parseScene("data/myscene_PlayStage.scene", this->root, "PlayStage");
	parseScene("data/myscene_MenuStage.scene", this->rootMenu, "MenuStage");
	parseScene("data/myscene.scene", this->root, "PlayStage");

	audios.push_back(new Audio("data/audio/pickupKeys.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/talking.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/click.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/openDoor.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/button.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/closeDoor.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/MusicaIntro.wav", true, 0.05f));
	audios.push_back(new Audio("data/audio/musicaFondo.wav", true, 0.04f));
	audios.push_back(new Audio("data/audio/win.wav", true, 0.02f));
	audios.push_back(new Audio("data/audio/start.wav", false, 0.2f));
	audios.push_back(new Audio("data/audio/blueLabel.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/burguer.wav", false, 0.5f));
	audios.push_back(new Audio("data/audio/reloj.wav", true, 0.5f));
	audios.push_back(new Audio("data/audio/perder.wav", false, 0.6f));
	//std::cout << root.children.size() << std::endl;
}

void World::render()
{

}

void World::update(float dt)
{


}

bool World::parseScene(const char* filename, Entity &ent, std::string stage)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors

	meshes_to_load.clear();



	meshes_to_load["data/PlayStage/meshes/Plane.obj"] = { Texture::Get("data/floor.png"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	meshes_to_load["data/MenuStage/meshes/Plane.obj"] = { Texture::Get("data/floor.png"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };



	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load["data/" + stage + "/" + mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities


	for (auto data : meshes_to_load) {

		mesh_name = data.first;
		
		sRenderData& render_data = data.second;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			//InstancedEntityMesh* new_entity = new InstancedEntityMesh(Mesh::Get(mesh_name.c_str()), render_data.shader, render_data.texture);
			//InstancedEntityMesh* new_entity = new InstancedEntityMesh(Mesh::Get(mesh_name.c_str()), shader, texture);

		}
		// Create normal entity
		else {
			//EntityMesh* new_entity = new EntityMesh(NULL, "ent", Mesh::Get(mesh_name.c_str()), render_data.texture, render_data.shader, Vector3());
			if(render_data.shader == NULL)
			{
				render_data.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			}
			if (render_data.texture == NULL)
			{
				render_data.texture = Texture::Get("data/pared.png");
			}


			EntityMesh* new_entity = new EntityMesh(NULL, "ent", Mesh::Get(mesh_name.c_str()), render_data.texture, render_data.shader, Vector3());
			std::cout << mesh_name << std::endl;
			if (render_data.texture == Texture::Get("data/floor.png"))
			{
				new_entity->tiling = 50.f;
				new_entity->cull = false;

				//floor = new_entity;
			}
			else {
				new_entity->tiling = 1.f;
				//entities.push_back(new_entity);
			}
			new_entity->isCollider = true;
			if (stage == "MenuStage") { new_entity->cull = false;  }
			new_entity->model = render_data.models[0];
			// Add entity to scene root
			ent.addChild(new_entity);
		}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;

	return true;
}

