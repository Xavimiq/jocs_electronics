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
	Shader* shader;
	Mesh* mesh;
	
	
	struct Material {
		Shader* shader = nullptr;
		Vector4 color = Vector4(1.f);

		Texture* diffuse = nullptr;
		Texture* normals = nullptr;
		
		Vector3 Ks = Vector3(0.2);

		bool transparent = false;
		float tiling = 1.f;
	};
	Material landscape_cubemap;




	Camera* gui_camera;
	World();

	struct sRenderData {
		Texture* texture = nullptr;
		Shader* shader = nullptr;
		std::vector<Matrix44> models;
	};

	EntityMesh* landscape;
	Entity root;
	Entity rootMenu;

	//EntityMesh* floor;
	std::map<std::string, sRenderData> meshes_to_load;

	EntityMesh* sky;
	//Entity root;
	//EntityMesh* e_mesh;
	
	EntityMesh* key;
	

	
	Vector3 cameraCurrentPos;
	Camera* camera;


	bool freeCam = false;

	void render();
	void update(float dt);
	bool parseScene(const char* filename, Entity &ent, std::string stage);

	std::vector<Vector3> debug_col_positions;



	std::vector<Vector3> waypoints;

};

