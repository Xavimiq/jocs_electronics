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

	//new camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 0.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, width / (float)height, 0.1f, 10000.f);

	/*
	Texture* texture;
	texture = new Texture();
	texture->load("data/texture.tga");

	Mesh* mesh;
	mesh = Mesh::Get("data/box.ASE");
	//mesh2 = Mesh::Get("data/untitled.ASE");


	// example of shader loading using the shaders manager
	Shader* shader;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	e_mesh = new EntityMesh(NULL, "ent", mesh, texture, shader, Vector3(100, 1, 1));
	root.addChild(e_mesh);
	*/

	//RENDER SKY
	
	
	Texture* texture_sky;
	texture_sky = new Texture();
	texture_sky->load("data/sky/clouds2.tga");

	Mesh* mesh_sky;
	mesh_sky = Mesh::Get("data/sky/cielo.ASE");

	Shader* shader_sky;
	shader_sky = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	sky = new EntityMesh(NULL, "sky", mesh_sky, texture_sky, shader_sky, Vector3(0, 0, 0));
	root.addChild(sky);

	///RENDER KEY

	Texture* texture_key;
	texture_key = new Texture();
	texture_key->load("data/texture.tga");

	Mesh* mesh_key;
	mesh_key = Mesh::Get("data/key/key.obj");

	Shader* shader_key;
	shader_key = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");

	key = new EntityMesh(NULL, "key", mesh_key, texture_key, shader_key, Vector3(0, 0, 0));
	root.addChild(key);
	


	///RENDER ENEMY

	Texture* texture_enemy;
	texture_enemy = new Texture();
	texture_enemy->load("data/texture.tga");

	Mesh* mesh_enemy;
	mesh_enemy = Mesh::Get("data/Enemy/enemy.obj");

	Shader* shader_enemy;
	shader_enemy = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs");

	//enemy = new EntityMesh(NULL, "enemy", mesh_enemy, texture_enemy, shader_enemy, Vector3(50, 0, 50));

	my_enemy = new EntityEnemy("ene", Vector3(50, 0, 200), mesh_enemy, shader_enemy, texture_enemy);

	//root.addChild(my_enemy);


	my_player = new EntityPlayer(Vector3(0, 10, 2));
	

	parseScene("data/myscene.scene");

	//freeCam = true;

}

void World::render()
{
	//if (!freeCam)
	//{
	//	camera->lookAt(
	//		cameraCurrentPos,
	//		e_mesh->getGlobalMatrix() * Vector3(0.f, 6.f, -50.f),
	//		e_mesh->getGlobalMatrix().rotateVector(Vector3(0, 1, 0))
	//	);
	//}
	

	camera->enable();
	
	//CAMERA ORIENTATION
	Matrix44 mYaw;
	mYaw.setRotation(my_player->yaw, Vector3(0, 1, 0));
	Matrix44 mPitch;
	mPitch.setRotation(my_player->pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector();
	Vector3 eye;
	Vector3 center;

	eye = my_player->getGlobalMatrix() * Vector3(0.f, 22.f, 0.5f);
	center = eye + front;

	camera->lookAt(eye, center, Vector3(0, 1, 0));
	//camera->eye = my_player->position;
	//sky->model.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	
	//FRUSTUM TO-CHECK

	//std::vector<Entity> entities;
	//entities.push_back(root);

	//for (int i = 0; i < root.size(); i++) {
	//	EntityMesh obj = static_cast<EntityMesh>(entities[i]);

	//	Vector3 sphere_center = obj->model obj->mesh->box.center;
	//	float sphere_radius = obj->mesh->radius;

	//	if (camera->testSphereInFrustum(sphere_center, sphere_radius) == false ||
	//		camera->eye.distance(obj->model.getTranslation()) > 500) {
	//		continue;
	//	}
	//	obj->render();
	//}


	root.render();

	my_enemy->render();
}

void World::update(float dt)
{
	my_player->update(dt);
	
	Matrix44& playerModel = my_player->model;
	Matrix44 mYaw;
	mYaw.setRotation(my_player->yaw, Vector3(0, 1, 0));
	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();
	

	Vector3 position = playerModel.getTranslation();

	Vector3 move_dir;
	move_dir = Vector3();
	

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir = move_dir + front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir = move_dir - front;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir = move_dir + right;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir = move_dir - right;
	
	
	if((move_dir.x != 0) && (move_dir.y != 0) && (move_dir.z != 0)) move_dir.normalize();
	


	my_player->velocity = my_player->velocity + move_dir * my_player->speed;
		
	bool isOnFloor = false;
	//

	//---------------------------------- COLLISIONS
	std::vector<sCollisionData> collisions;

	if (checkPlayerCollision(position + my_player->velocity * dt * 10.0f, collisions))
	{
		float up_factor = 0.0f;  // Inicializar a 0.0f aquí
		for (sCollisionData& collision : collisions)
		{
			up_factor = collision.colNormal.dot(Vector3(0, 1, 0));
			//std::cout << "Vector x: " << collision.colNormal.x << std::endl;
			//std::cout << "Vector y: " << collision.colNormal.y << std::endl;
			//std::cout << "Vector z: " << collision.colNormal.z << std::endl;
			//std::cout << up_factor << std::endl;
			if (up_factor > 0.8)
			{
				isOnFloor = true;
				//std::cout << isOnFloor << std::endl;
			}
			else {
				Vector3 newDir = my_player->velocity.dot(collision.colNormal) * collision.colNormal;
				my_player->velocity.x = my_player->velocity.x - newDir.x;
				my_player->velocity.z = my_player->velocity.z - newDir.z;
			}
		}

	}

	if (!isOnFloor)
	{
		my_player->velocity.y = my_player->velocity.y - 2.5f * dt * 20.f;// *10.f;
		//printf("hola");
	}
	else {
		if (Input::isKeyPressed(SDL_SCANCODE_SPACE)){
			my_player->velocity.y = my_player->jump_speed;
		}
		else
		{
			my_player->velocity.y = 0.f;
		}
	}

	position = position + my_player->velocity * dt * 10.f;

	// Freno
	my_player->velocity.x = my_player->velocity.x * 0.5f;
	my_player->velocity.z = my_player->velocity.z * 0.5f;

	// Actualizar
	playerModel.setTranslation(position.x, position.y, position.z);
	playerModel.rotate(my_player->yaw, Vector3(0, 1, 0));

	//float high_camera 
	//camera->eye.y = high_camera;

	if (checkLineOfSight(my_enemy->model, my_player->model))
	{ 
		my_enemy->color = Vector4(1.f, 0.f, 0.f, 1.f);
		std::cout << "target spotted!!!!!!" << std::endl;
	}

	root.update(dt);
	my_enemy->update(dt);


}

bool World::parseScene(const char* filename)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors
	 //meshes_to_load["meshes/example.obj"] = { Texture::Get("texture.tga"), Shader::Get("shader.vs", "shader.fs") };

	Texture* texture;
	Shader* shader;


	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	texture = new Texture();
	texture->load("data/texture.tga");



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
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			//InstancedEntityMesh* new_entity = new InstancedEntityMesh(Mesh::Get(mesh_name.c_str()), render_data.shader, render_data.texture);
			InstancedEntityMesh* new_entity = new InstancedEntityMesh(Mesh::Get(mesh_name.c_str()), shader, texture);


			// Add all instances
			new_entity->models = render_data.models;
			// Add entity to scene root
			//root.addChild(new_entity);
			root.addChild(new_entity);
		}
		// Create normal entity
		else {
			//EntityMesh* new_entity = new EntityMesh(NULL, "ent", Mesh::Get(mesh_name.c_str()), render_data.texture, render_data.shader, Vector3());
			EntityMesh* new_entity = new EntityMesh(NULL, "ent", Mesh::Get(mesh_name.c_str()), texture, shader, Vector3());

			new_entity->model = render_data.models[0];
			// Add entity to scene root
			root.addChild(new_entity);
		}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}

bool World::checkPlayerCollision(Vector3& target_position, std::vector<sCollisionData>& collisions)
{
	Vector3 position = my_player->model.getTranslation();
	Vector3 character_center = target_position + Vector3(0.f, 1.4f, 0.f);
	float sphereRadius = 3.f;

	Vector3 colPoint;
	Vector3 colNormal;
	for (auto e : root.children)
	{
		EntityMesh* em_e = dynamic_cast<EntityMesh*>(e);
		if (!em_e)
			continue;

		Mesh* mesh = em_e->mesh;

		InstancedEntityMesh* instancedEntity = dynamic_cast<InstancedEntityMesh*>(e);
		if(instancedEntity != nullptr){
			printf("Isinstanced");
		
		}
		else {
			if (mesh->testSphereCollision(e->model, character_center, sphereRadius, colPoint, colNormal))
			{
				collisions.push_back({ colPoint, colNormal.normalize() });
			}


			if (mesh->testRayCollision(e->model, character_center, Vector3(0, -1, 0), colPoint, colNormal, 2.9f))
			{
				collisions.push_back({colPoint, colNormal.normalize()});
				//std::cout << colPoint.x << std::endl;
			}
		}
		
	}
	return !collisions.empty();
}

bool World::checkLineOfSight(Matrix44& obs, Matrix44& target) 
{
	Vector3 front = normalize(obs.frontVector());
	Vector3 toTarget = target.getTranslation() - obs.getTranslation();

	float distance = toTarget.length();
	toTarget = toTarget.normalize();

	Vector3 ray_origin = obs.getTranslation();
	Vector3 ray_direction = toTarget;

	if (toTarget.dot(front) > 0.5)
	{
		for (auto e : root.children)
		{
			EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
			if (!ec) continue;
		
			if (ec->mesh->testRayCollision(
				ec->model,
				ray_origin,
				ray_direction,
				Vector3(),
				Vector3(),
				distance
			)) {
				return false; //NO COLISIONA
			}

		}
		return true;
	}
	return false;
}