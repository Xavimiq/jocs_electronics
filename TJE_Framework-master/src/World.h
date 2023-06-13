#pragma once

#include "framework.h"
#include "Entity.h"
#include "camera.h"
#include "shader.h"



class World
{

	static World* instance;
	

public:

	static World* get_instance()
	{
		if (instance != nullptr) return instance;
		instance = new World();
		return instance;
	}


	World();

	struct sRenderData {
		Texture* texture = nullptr;
		Shader* shader = nullptr;
		std::vector<Matrix44> models;
	};

	Entity root;
	std::map<std::string, sRenderData> meshes_to_load;


	//Entity root;
	EntityMesh* e_mesh;
	EntityMesh* sky;
	EntityMesh* key;
	EntityMesh* enemy;
	
	Vector3 cameraCurrentPos;
	Camera* camera;
	EntityPlayer* my_player;
	EntityEnemy* my_enemy;

	bool freeCam = false;

	void render();
	void update(float dt);
	bool parseScene(const char* filename, Entity &ent, std::string stage);

	std::vector<Vector3> debug_col_positions;

	struct sCollisionData {
		Vector3 colPoint;
		Vector3 colNormal;
	};

	bool checkPlayerCollision(Vector3& target_position, std::vector<sCollisionData>& collisions);
	bool checkLineOfSight( Matrix44& obs,  Matrix44& target);

	std::vector<Vector3> waypoints;

};

