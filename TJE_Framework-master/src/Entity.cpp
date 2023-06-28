#include "Entity.h"
#include "game.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "framework.h"
#include "input.h"
#include "World.h"

Entity::Entity()
{
	gameInstance = Game::instance;
	this->name = "Ent";
	this->parent = NULL;


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


EntityMesh::EntityMesh(Entity* parent, std::string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 pos):Entity(pos, NULL)
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
	Matrix44 globalmatrix = this->getGlobalMatrix();

	Vector3 sphere_center = globalmatrix * mesh->box.center;

	float sphere_radius = mesh->radius;

	if (cull && (camera->testSphereInFrustum(sphere_center, sphere_radius) == false ||
		camera->eye.distance(globalmatrix.getTranslation()) > 500)) {
		Entity::render();
		return;
	}

	camera->enable();

	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", globalmatrix);
	shader->setUniform("u_time", time);
	shader->setUniform("u_tiling", tiling);

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

};

InstancedEntityMesh::InstancedEntityMesh(Mesh* mesh, Shader* shader, Texture* texture) 
	:EntityMesh(NULL, name, mesh, texture, shader, Vector3()) 
{ 
	
	isInstanced = true; 	
	this->models.push_back(this->model);
};

void InstancedEntityMesh::render()
{
	EntityMesh::render();
};

void InstancedEntityMesh::update(float dt)
{
	EntityMesh::update(dt);
};



EntityPlayer::EntityPlayer(Vector3 spawn_point) : Entity(spawn_point, NULL) {
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;

	my_camera = new Camera();
	my_camera->lookAt(Vector3(0.f, 0.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	my_camera->setPerspective(70.f, width / (float)height, 0.1f, 10000.f);

	this->position = Vector3(spawn_point.x, 30, spawn_point.z);
};

void EntityPlayer::render() {

	my_camera->enable();

	Matrix44 mYaw;
	mYaw.setRotation(yaw, Vector3(0, 1, 0));
	Matrix44 mPitch;
	mPitch.setRotation(pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector();
	Vector3 eye;
	Vector3 center;



	if (Input::isKeyPressed(SDL_SCANCODE_W) && Input::isKeyPressed(SDL_SCANCODE_X) && Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
	{
		eye = getGlobalMatrix() * Vector3(0.f, 20.f, 0.5f);
		center = eye + front;
		isSliding = true;
	}
	else {
		eye = getGlobalMatrix() * Vector3(0.f, 37.5f, 0.5f);
		center = eye + front;
		isSliding = false;
	}

	my_camera->lookAt(eye, center, Vector3(0, 1, 0));

	//MIRAR ATRÁS
	if (Input::isKeyPressed(SDL_SCANCODE_C)) my_camera->lookAt(my_camera->eye, eye - front, Vector3(0, 1, 0));

	Entity::render();
};

void EntityPlayer::update(float dt) {

	Input::centerMouse();
	yaw -= Input::mouse_delta.x * dt * 10.0f * DEG2RAD;
	pitch -= Input::mouse_delta.y * dt * 10.0f * DEG2RAD;

	pitch = clamp(pitch, -M_PI * 0.25f, M_PI * 0.25f);

	Matrix44& playerModel = model;
	Matrix44 mYaw;
	mYaw.setRotation(yaw, Vector3(0, 1, 0));
	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();


	Vector3 position = playerModel.getTranslation();

	Vector3 move_dir;
	move_dir = Vector3();


	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir = (move_dir + front) * 0.25f;
	if ((Input::isKeyPressed(SDL_SCANCODE_W) && Input::isKeyPressed(SDL_SCANCODE_LSHIFT))) move_dir = (move_dir + front) * 0.45f;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir = (move_dir - front) * 0.25f;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir = (move_dir + right) * 0.25f;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir = (move_dir - right) * 0.25f;

	if (Input::isKeyPressed(SDL_SCANCODE_E))
	{
		std::cout << model.getTranslation().x << "-------------+" << model.getTranslation().z << std::endl;
	}

	if (model.getTranslation().x < 1875 && model.getTranslation().x > 1850 &&
		model.getTranslation().z < 80 && model.getTranslation().z > 50) {
		std::cout << Game::instance->current_stage << std::endl;
		Game::instance->current_stage = Win;
	}

	velocity = velocity + move_dir * speed;

	bool isOnFloor = false;

	//---------------------------------- COLLISIONS
	std::vector<sCollisionData> collisions;

	if (checkPlayerCollision(position + velocity * dt * 10.0f, collisions))
	{
		float up_factor = 0.0f;  // Inicializar a 0.0f aquí
		for (sCollisionData& collision : collisions)
		{
			up_factor = collision.colNormal.dot(Vector3(0, 1, 0));

			if (up_factor > 0.8)
			{
				isOnFloor = true;
			}
			else {
				Vector3 newDir = velocity.dot(collision.colNormal) * collision.colNormal;
				velocity.x = velocity.x - newDir.x;
				velocity.z = velocity.z - newDir.z;
			}
		}

	}

	if (!isOnFloor)
	{
		velocity.y = velocity.y - 2.5f * dt * 7.5f;
	}
	else {
		if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) {
			velocity.y = jump_speed;
		}
		else
		{
			velocity.y = 0.f;
		}
	}

	position = position + velocity * dt * 10.f;

	// Freno
	velocity.x = velocity.x * 0.5f;
	velocity.z = velocity.z * 0.5f;

	// Actualizar
	playerModel.setTranslation(position.x, position.y, position.z);
	playerModel.rotate(yaw, Vector3(0, 1, 0));


	Entity::update(dt);

}

bool EntityPlayer::checkPlayerCollision(Vector3& target_position, std::vector<sCollisionData>& collisions)
{
	Vector3 position = model.getTranslation();
	if (!isSliding) { character_center = target_position + Vector3(0.f, 14.f, 0.f); }
	else{ Vector3 character_center = target_position + Vector3(0.f, 5.f, 0.f); }
	
	float sphereRadius = 3.f;

	Vector3 colPoint;
	Vector3 colNormal;
	for (auto e : World::get_instance()->root.children)
	{
		EntityMesh* em_e = dynamic_cast<EntityMesh*>(e);
		if (!em_e)
			continue;

		Mesh* mesh = em_e->mesh;

		InstancedEntityMesh* instancedEntity = dynamic_cast<InstancedEntityMesh*>(e);
		if (instancedEntity != nullptr) {
			printf("Isinstanced");

		}
		else {
			if (mesh->testSphereCollision(e->model, character_center, sphereRadius, colPoint, colNormal))
			{
				collisions.push_back({ colPoint, colNormal.normalize() });
			}


			if (mesh->testRayCollision(e->model, character_center, Vector3(0, -1, 0), colPoint, colNormal, 2.9f))
			{
				collisions.push_back({ colPoint, colNormal.normalize() });
				//std::cout << colPoint.x << std::endl;
			}
		}

	}
	return !collisions.empty();
}

void EntityEnemy::render()
{
	if (!mesh) return;
	Camera* camera = Camera::current;
	Matrix44 globalmatrix = this->getGlobalMatrix();

	Vector3 sphere_center = globalmatrix * mesh->box.center;

	float sphere_radius = mesh->radius;

	if (cull && (camera->testSphereInFrustum(sphere_center, sphere_radius) == false ||
		camera->eye.distance(globalmatrix.getTranslation()) > 500)) {
		Entity::render();
		return;
	}

	camera->enable();

	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", globalmatrix);
	shader->setUniform("u_time", time);
	shader->setUniform("u_tiling", tiling);

	shader->setUniform("u_camera_pos", camera->eye);
	shader->setUniform("u_light_pos", Vector3(400, 0, 0));



	shader->setUniform("u_Ka", Vector3(1.f));
	shader->setUniform("u_Kd", Vector3(1.f));
	shader->setUniform("u_Ks", Vector3(1.f));


	mesh->renderAnimated(GL_TRIANGLES, &anim_state.getCurrentSkeleton());

	//disable shader
	shader->disable();

	Entity::render();
}


void EntityEnemy::update(float elapsed_time)
{
	behavior_c.update(elapsed_time);
	anim_state.update(elapsed_time);
}

EntityGUIelement::EntityGUIelement(Vector2 pos, Vector2 size, Texture* texture, eButtonId button_id, const std::string name)
	: EntityMesh(NULL, name, nullptr, texture, nullptr, Vector3())
{
	position = pos;
	this->size = size;
	this->button_id = button_id;

	mesh = new Mesh();
	mesh->createQuad(pos.x, pos.y, size.x, size.y, false);

	shader = Shader::Get("data/shaders/basic_gui.vs", "data/shaders/texture.fs");
	this->texture = texture;
}


void EntityGUIelement::render() {

	if (!visible)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->enable();

	Matrix44 viewProj;

	if (use_pixel_coords) {
		viewProj = World::get_instance()->gui_camera->viewprojection_matrix;
	}

	//upload uniforms
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", viewProj);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_color", Vector4(1.f));

	shader->setUniform("u_tiling", 1.0f);

	mesh->render(GL_TRIANGLES);

	shader->disable();

	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


}

void EntityGUIelement::update(float dt) {
	
	Vector2 mouse_position = Input::mouse_position;

	if (button_id != NoButton &&
		mouse_position.x > (position.x - size.x * 0.5) &&
		mouse_position.x < (position.x + size.x * 0.5) &&
		mouse_position.y >(position.y - size.y * 0.5) &&
		mouse_position.y < (position.y + size.y * 0.5)) {

		color = Vector4(1, 0, 0, 1);

		if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			if (button_id == playButton) { Game::instance->current_stage = Play ;}
		}
		if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			if (button_id == exitButton) { Game::instance->current_stage = Exit; }
		}
	}
	else {
		color = Vector4(1);
	}
	
	Entity::update(dt);
}



EntityPrisoner::EntityPrisoner(std::string name, Vector3 pos, Mesh* mesh, Shader* shader, Texture* texture, Texture* texture_dialogue) : EntityMesh(nullptr, name, mesh, texture, shader, pos)
{
	Dialogue = new EntityGUIelement(Vector2(width / 2, height/2), Vector2(300, 300), texture_dialogue, eButtonId::Dialogue, "ent_diag");
	GUIelements.addChild(Dialogue);
}

void EntityPrisoner::talkwithplayer() 
{
	GUIelements.render();
}

void EntityPrisoner::update(float dt) {};


EntityKey::EntityKey(Vector3 pos, EntityMesh* _Door) :
	EntityMesh(nullptr, "ent_key", Mesh::Get("data/PlayStage/key.obj"), Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs"), pos)
{
	Texture* t_key;
	t_key = new Texture();
	t_key->load("data/PlayStage/key.png");

	Door = _Door;
	KeyImage = new EntityGUIelement(Vector2(width / 8, height - 100), Vector2(100, 100), t_key, eButtonId::Key, "ent_diag");
	GUIelements.addChild(KeyImage);
}



void EntityKey::collectKey(EntityPlayer* player)
{
	if ((player->model.getTranslation() - this->model.getTranslation()).length() < 50.f && Input::wasKeyPressed(SDL_SCANCODE_T)) {
		this->model.setTranslation(0.f, -60.f, 0.f);
		isCollected = true;
		std::cout << "llave seleccionada" << std::endl;
		Door->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		Door->model.translate(17, 0, 17);
	}
	if (isCollected)
	{	
		GUIelements.render();
	}

}

EntityButtonDoor::EntityButtonDoor(Vector3 ButtonPosition, Vector3 DoorPosition, float time_to_close, EntityPlayer* player, std::vector<Vector3> area) : Entity(DoorPosition, NULL)
{
	//if (sizeof(area) != 2) return;
	this->Door = new EntityDoor(DoorPosition);
	this->DoorTime = time_to_close;
	this->player = player;
	this->ButtonPosition = ButtonPosition;
	this->area = area;

	this->ButtonNotPressed = new EntityMesh(nullptr, "ent_bot", Mesh::Get("data/PlayStage/BottonNormal.obj"), Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs"), ButtonPosition);
	this->ButtonPressed = new EntityMesh(nullptr, "ent_bot", Mesh::Get("data/PlayStage/BottonPush.obj"), Texture::Get("data/texture.tga"), Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color_mat.fs"), ButtonPosition);

}

void EntityButtonDoor::render() {
	Door->render();
	if (isPressed) {
		this->ButtonPressed->render();
	}
	else {
		this->ButtonNotPressed->render();
	}
}

void EntityButtonDoor::clickBotton() {
	if (isPressed)	return;
	if ((this->player->model.getTranslation() - this->ButtonPosition).length() < 50.f && Input::wasKeyPressed(SDL_SCANCODE_T)) {
		isPressed = true;
		time_to_go = DoorTime;

		this->Door->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		Door->model.translate(17, 0, 17);
	}

}

void EntityButtonDoor::OpenDoor(float dt) {
	const Vector3& punto1 = area[0];
	const Vector3& punto2 = area[1];


	if (isPressed) {
		time_to_go -= dt;

		if (time_to_go <= 0.0f) {
			isPressed = false;
			this->Door->model.translate(-17, 0, -17);
			this->Door->model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));
			if (punto1.x <= this->player->model.getTranslation().x && this->player->model.getTranslation().x <= punto2.x &&
				punto1.y <= this->player->model.getTranslation().y && this->player->model.getTranslation().y <= punto2.y &&
				punto1.z <= this->player->model.getTranslation().z && this->player->model.getTranslation().z <= punto2.z) {

				std::cout << "YOU LOST";
				Game::instance->current_stage = Lost;
			}
		}
	}
}
