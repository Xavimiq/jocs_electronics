#ifndef ENTITY_H
#define ENTITY_H


#include "framework.h"
#include "mesh.h"
#include "texture.h"


class Game;

class Entity
{
private:
	Game* gameInstance = nullptr;
public:
	Entity(Entity* parent);
	Entity(Vector3 pos, Entity* parent = NULL);
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

	EntityMesh(Entity* parent, std::string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 pos);

	void render() override;
	void update(float elapsed_time) override;

	
};


#endif

