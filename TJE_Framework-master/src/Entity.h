#ifndef ENTITY_H
#define ENTITY_H


#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "ai_behavior.h"

class Game;

class Entity
{
private:
	Game* gameInstance = nullptr;
public:
	Entity();
	Entity(Vector3 pos, Entity* parent);
	virtual ~Entity();

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

	EntityMesh(Entity* parent, std::string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 pos);

	void render() override;
	void update(float elapsed_time) override;

	bool isInstanced = false;

	
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

	float speed = 10.f;
	float jump_speed = 10.f;

	EntityPlayer(Vector3 spawn_point);

	//void jump();

	void render() override;
	virtual void update(float dt) override;

	
};

class EntityEnemy : public EntityMesh {

public:
	//EntityEnemy() {};
	EntityEnemy(std::string name, Vector3 pos, Mesh* mesh, Shader* shader, Texture* texture = nullptr) :
		EntityMesh(NULL, name, mesh, texture, shader, pos) {behavior.setModel(&model);}

	AIBehavior behavior;

	void update(float elapsed_time);
};

class EntityCollider : public EntityMesh {

	//void getCollisionsWithModel(Matrix44& m, Vector3& center, std::vector<sCol )

public:
	//EntityCollider() {};
	//EntityCollider(Mesh* mesh, Shader* shader, Texture* texture = nullptr, std)

	~EntityCollider() {};

	//int layer = sCollisionFilter::SCENARIO;
	
	bool isStatic = true;
	float sphereRadius = 0.75f;
	float playerHeight = 1.4f;

	//void getCollisions(Vector3& target_position, std)
};


#endif