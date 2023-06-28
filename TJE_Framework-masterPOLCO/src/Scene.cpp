#include "Scene.h"
#include "Entity.h"


Scene::Scene()
{
	this->setRoot(new Entity());
}

void Scene::render()
{
	if (!this || !this->root) return;
	this->root->render();
}

void Scene::update(float elapsed_time) {
	if (this->root == nullptr) return;
	this->root->update(elapsed_time);
}

Entity* Scene::getRoot() {
	return root;
}

void Scene::setRoot(Entity* root) {
	this->root = root;
};

