#ifndef ENTITY_H
#define ENTITY_H


#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "ai_behavior.h"
#include "shader.h"
#include "game.h"

class Game;
//class AIBehavior;

class Entity
{
private:
	Game* gameInstance = nullptr;
public:
	Entity();
	Entity(Vector3 pos, Entity* parent);
	virtual ~Entity();
	
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	

	std::string name;
	Matrix44 model;
	Matrix44 globalModel;


	virtual void render();
	virtual void update(float elapsed_time);

	Entity* parent;

	
	// Pointers to children
	std::vector<Entity*> children;

	void setParent(Entity* parentObj);
	void addChild(Entity* child);
	//void removeChild(Entity* child);

	Matrix44 getGlobalMatrix();
	void rotate(float angle_in_rad, const Vector3& axis);
};

class EntityMesh : public Entity {

public:

	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;
	std::vector<Matrix44> models;
	

	float tiling = 1.f;

	bool cull = true;

	EntityMesh(Entity* parent, std::string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 pos);

	void render() override;
	void update(float elapsed_time) override;

	bool isInstanced = false;
	bool isCollider = false;
	
};

class InstancedEntityMesh : public EntityMesh {
	
public:
	//InstancedEntityMesh();//{ isInstanced = true; };
	InstancedEntityMesh(Mesh* mesh, Shader* shader, Texture* texture);
	~InstancedEntityMesh() {};

	std::vector<Matrix44> models;

	void render() override;
	void update(float dt) override;

};


class EntityPlayer : public Entity {

public:
	Vector3 position;
	
	Vector3 velocity;
	float yaw = 0.f;
	float pitch = 0.f;

	bool isSliding = false;
	Vector3 character_center;

	float speed = 10.f;
	float jump_speed = 7.f;

	Camera* my_camera;

	EntityPlayer(Vector3 spawn_point);


	struct sCollisionData {
		Vector3 colPoint;
		Vector3 colNormal;
	};

	void render() override;
	virtual void update(float dt) override;

	bool checkPlayerCollision(Vector3& target_position, std::vector<sCollisionData>& collisions);
	//bool checkLineOfSight(Matrix44& obs, Matrix44& target);

	
};

class EntityEnemy : public EntityMesh {

public:
	AIBehavior behavior_c;
	//EntityEnemy() {};
	EntityEnemy(std::string name, Vector3 pos, Mesh* mesh, Shader* shader, Texture* texture, std::vector<Vector3> waypoints, EntityPlayer* player) :
		EntityMesh(nullptr, name, mesh, texture, shader, pos) 
	{
		this->behavior_c.setModel(&model);
		this->behavior_c.waypoints = waypoints;
		this->behavior_c.my_player = player;
	}

	void update(float elapsed_time);
};


enum eButtonId { NoButton, Button, playButton, exitButton, Dialogue, Key};

class EntityGUIelement : public EntityMesh {

public:

	EntityGUIelement(Vector2 pos, Vector2 size, Texture* texture, eButtonId button_id, const std::string name);

	Vector2 position;
	Vector2 size;

	bool use_pixel_coords = true;
	bool is3D = false;
	bool selected = false;
	bool visible = true;
	
	eButtonId button_id = NoButton;

	void render();
	void update(float dt);
	void update3D(float dt);
};

class EntityPrisoner : public EntityMesh {

public:
	Entity GUIelements;
	EntityGUIelement* Dialogue;

	EntityPrisoner(std::string name, Vector3 pos, Mesh* mesh, Shader* shader, Texture* texture, Texture* texture_dialogue);
		
	void update(float dt);
	void talkwithplayer();

};

class EntityKey : public EntityMesh {

public:
	Entity GUIelements;
	EntityGUIelement* KeyImage;
	EntityMesh* Door;



	bool isCollected = false;

	EntityKey(Vector3 pos, EntityMesh* _Door);

	void collectKey(EntityPlayer* player);
	//void openDoor(EntityPlayer* player, );

};

class EntityDoor : public EntityMesh {

public:
	EntityDoor(Vector3 pos) : EntityMesh(nullptr, "ent_door", Mesh::Get("data/PlayStage/Door.obj"), Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs"), pos) {};

};



class EntityBottonDoor : public EntityMesh{

public:
	EntityDoor* Door1;

	EntityBottonDoor(Vector3 BottonPosition, Vector3 DoorPosition, float time_to_close);

	void OpenDoor(EntityPlayer* player, float dt);

};

#endif