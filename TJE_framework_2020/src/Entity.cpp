#include "Entity.h"
#include "game.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "framework.h"

Entity::Entity(Entity* parent)
{
	gameInstance = Game::instance;
	this->name = "Ent";
	this->parent = parent;

}

Entity::Entity(Vector3 pos, Entity* parent)
{
	this->model.setIdentity();
	this->model.translate(pos.x, pos.y, pos.z);
	this->name = "ent";
	this->parent = parent;


}

Entity::~Entity()
{

}


void Entity::render() {

	// Render code if EntityMesh
// ...

	for (int i = 0; i < children.size(); i++)
		children[i]->render();

}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent == NULL)
		return model * Matrix44::IDENTITY;
	else
		return model * parent->getGlobalMatrix();
}

void Entity::update(float elapsed_time)
{
	this->globalModel = getGlobalMatrix();

	if (!children.size()) return;
	for (int i = 0; i < children.size(); ++i) 
	{
		children[i]->update(elapsed_time);
	}
}

void Entity::setParent(Entity* parentObj)
{
	this->parent = parentObj;
}

void Entity::addChild(Entity* ent)
{
	this->children.push_back(ent);
	ent->setParent(this);

}

void Entity::rotate(float angle_in_rad, const Vector3& axis)
{
	Matrix44 R;
	R.setRotation(angle_in_rad, axis);
	this->model = R * this->model;
}


EntityMesh::EntityMesh(Entity* parent, std::string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 pos):Entity(pos)
{
	this->parent = parent;
	this->name = name;
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
}

void EntityMesh::render()
{

	Camera* camera = Camera::current;
	Matrix44 model = this->getGlobalMatrix();

	camera->enable();

	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", time);

	shader->setUniform("u_camera_pos", camera->eye);
	shader->setUniform("u_light_pos", Vector3(400, 0, 0));

	//do the draw call
	mesh->render( GL_TRIANGLES );


	//disable shader
	shader->disable();

	Entity::render();
}

void EntityMesh::update(float dt)
{

	Entity::update(dt);

}



