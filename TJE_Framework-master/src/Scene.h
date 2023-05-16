#pragma once

#include "framework.h"
#include "camera.h"

class Entity;

class Scene
{
public:
	Camera camera;
	Entity* root = nullptr;
	std::vector<Entity*> entities;

	Scene();

	void render();
	void update(float elapsed_time);
	void setRoot(Entity* entity);
	Entity* getRoot();
};

