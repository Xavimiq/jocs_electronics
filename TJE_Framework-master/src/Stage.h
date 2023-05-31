#pragma once
#include "Entity.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game.h"
#include "Scene.h"
#include "camera.h"
#include "World.h"
#include "input.h"


class Stage
{
protected:
	Game* gameInstance;
	//Camera* new_camera;
	//Scene* scene = nullptr;
public:
	//eStageType stageType;
	Camera* my_camera;
	

	float delta_yaw;
	float delta_pitch;

	Stage();
	//Stage(Scene* scene);

	virtual void render();
	virtual void update(float seconds_elapsed);
	virtual void initStage() {};

	//Scene* getScene();
};

class PlayStage : public Stage {

private:
	EntityMesh* e_mesh;
	
	

public:

	//World* my_world;

	PlayStage();
	virtual void render() override;
	virtual void update(float elapsed_time) override;


};

class MenuStage : public Stage {

private:
	Entity* root;

public:
	float ang;

	Vector3 eye;
	Vector3 center;


	MenuStage();
	void render() override;
	void update(float elapsed_time) override;


};