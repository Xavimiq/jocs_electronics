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


	//std::cout << root.children.size() << std::endl;
}

void World::render()
{
	/*camera->enable();*/
	//landscape_cubemap.shader->enable();

	////upload uniforms
	//shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
	//shader->setUniform("u_Kd_texture", landscape_cubemap.diffuse, 0);

	//shader->setUniform("u_camera_projection", Camera::current->projection_matrix);

	
	////do the draw call
	//Mesh::Get("data/cubemap.ASE")->render(GL_TRIANGLES);


	////disable shader
	//landscape_cubemap.shader->disable();

	
	//CAMERA ORIENTATION
	//Matrix44 mYaw;
	//mYaw.setRotation(my_player->yaw, Vector3(0, 1, 0));
	//Matrix44 mPitch;
	//mPitch.setRotation(my_player->pitch, Vector3(-1, 0, 0));

	//Vector3 front = (mPitch * mYaw).frontVector();
	//Vector3 eye;
	//Vector3 center;



	//if (Input::isKeyPressed(SDL_SCANCODE_W) && Input::isKeyPressed(SDL_SCANCODE_X) && Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
	//{
	//	eye = my_player->getGlobalMatrix() * Vector3(0.f, 11.f, 0.5f);
	//	center = eye + front;

	//	//camera->lookAt(eye, center, Vector3(0, 1, 0));
	//}
	//else {
	//	eye = my_player->getGlobalMatrix() * Vector3(0.f, 22.f, 0.5f);
	//	center = eye + front;
	//}
	//camera->lookAt(eye, center, Vector3(0, 1, 0));

	////MIRAR ATRÁS
	//if (Input::isKeyPressed(SDL_SCANCODE_C)) camera->lookAt(camera->eye, eye - front, Vector3(0, 1, 0));


	//FRUSTUM TO-CHECK



	
	//
	//root.render();
	//floor->render();

	//if (waypoints.size() < 1)
	//{
	//	Mesh mesh;
	//	mesh.vertices = waypoints;

	//	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");

	//	shader->enable();

	//	//upload uniforms
	//	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//	shader->setUniform("u_model", Matrix44());

	//	//do the draw call
	//	mesh.render(GL_LINE_STRIP);


	//	//disable shader
	//	shader->disable();
	//}

	//root.render();
}

void World::update(float dt)
{
	/*my_player->update(dt);
	sky->model.setTranslation(my_player->model.getTranslation().x, my_player->model.getTranslation().y, my_player->model.getTranslation().z);*/


	//Matrix44& playerModel = my_player->model;
	//Matrix44 mYaw;
	//mYaw.setRotation(my_player->yaw, Vector3(0, 1, 0));
	//Vector3 front = mYaw.frontVector();
	//Vector3 right = mYaw.rightVector();
	//

	//Vector3 position = playerModel.getTranslation();

	//Vector3 move_dir;
	//move_dir = Vector3();
	//

	//if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir = (move_dir + front) * 0.5f;
	//if ((Input::isKeyPressed(SDL_SCANCODE_W) && Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) ) move_dir = (move_dir + front) * 0.75f;
	//if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir = (move_dir - front) * 0.5f;
	//if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir = (move_dir + right) * 0.5f;
	//if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir = (move_dir - right) * 0.5f;

	//if (Input::isKeyPressed(SDL_SCANCODE_E))
	//{
	//	std::cout << my_player->model.getTranslation().x << "-------------+" << my_player->model.getTranslation().z << std::endl;
 //	}
	//

	//my_player->velocity = my_player->velocity + move_dir * my_player->speed;
	//	
	//bool isOnFloor = false;

	////---------------------------------- COLLISIONS
	//std::vector<sCollisionData> collisions;

	//if (checkPlayerCollision(position + my_player->velocity * dt * 10.0f, collisions))
	//{
	//	float up_factor = 0.0f;  // Inicializar a 0.0f aquí
	//	for (sCollisionData& collision : collisions)
	//	{
	//		up_factor = collision.colNormal.dot(Vector3(0, 1, 0));
	//		//std::cout << "Vector x: " << collision.colNormal.x << std::endl;
	//		//std::cout << "Vector y: " << collision.colNormal.y << std::endl;
	//		//std::cout << "Vector z: " << collision.colNormal.z << std::endl;
	//		//std::cout << up_factor << std::endl;
	//		if (up_factor > 0.8)
	//		{
	//			isOnFloor = true;
	//			//std::cout << isOnFloor << std::endl;
	//		}
	//		else {
	//			Vector3 newDir = my_player->velocity.dot(collision.colNormal) * collision.colNormal;
	//			my_player->velocity.x = my_player->velocity.x - newDir.x;
	//			my_player->velocity.z = my_player->velocity.z - newDir.z;
	//		}
	//	}

	//}

	//if (!isOnFloor)
	//{
	//	my_player->velocity.y = my_player->velocity.y - 2.5f * dt * 20.f;// *10.f;
	//	//printf("hola");
	//}
	//else {
	//	if (Input::isKeyPressed(SDL_SCANCODE_SPACE)){
	//		my_player->velocity.y = my_player->jump_speed;
	//	}
	//	else
	//	{
	//		my_player->velocity.y = 0.f;
	//	}
	//}

	//position = position + my_player->velocity * dt * 10.f;

	//// Freno
	//my_player->velocity.x = my_player->velocity.x * 0.5f;
	//my_player->velocity.z = my_player->velocity.z * 0.5f;

	//// Actualizar
	//playerModel.setTranslation(position.x, position.y, position.z);
	//playerModel.rotate(my_player->yaw, Vector3(0, 1, 0));

	//root.update(dt);

	//COLLECT KEY

	//if ((my_player->model.getTranslation() - key->model.getTranslation()).length() < 50.f && Input::isKeyPressed(SDL_SCANCODE_T)) {
	//	key->model.setTranslation(0.f, -6.f, 0.f);
	//	//delete(key);
	//	std::cout << "llave seleccionada" << std::endl;
	//}

	//std::cout << camera->eye.x << camera->eye.y << camera->eye.z << std::endl;

}

bool World::parseScene(const char* filename, Entity &ent, std::string stage)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors

	meshes_to_load.clear();

	//meshes_to_load["data/PlayStage/meshes/Cube.001.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.002.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.004.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.005.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.006.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.007.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.009.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.010.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.011.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.012.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Cube.013.obj"] = { Texture::Get("data/pared.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };

	meshes_to_load["data/PlayStage/meshes/Plane.obj"] = { Texture::Get("data/floor.png"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Plane.001.obj"] = { Texture::Get("data/pared.png"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };
	//meshes_to_load["data/PlayStage/meshes/Plane.002.obj"] = { Texture::Get("data/pared.png"), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs") };


	//MENU
	//meshes_to_load["data/MenuStage/meshes/Cube.005.obj"] = { Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs") };
	//meshes_to_load["data/MenuStage/meshes/Cube.028.obj"] = { Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs") };
	//Texture* texture;
	//Shader* shader;


	//shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//texture = new Texture();
	//texture->load("data/pared.tga");



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


			// Add all instances
			//new_entity->models = render_data.models;
			// Add entity to scene root
			//root.addChild(new_entity);
			//root.addChild(new_entity);
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

