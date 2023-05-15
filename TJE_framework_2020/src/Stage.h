#pragma once
#include "Entity.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game.h"
#include "Scene.h"
#include "camera.h"


class Stage
{
protected:
	Game* gameInstance;
	Camera* new_camera;
	Scene* scene = nullptr;
public:
	//eStageType stageType;

	Stage();
	Stage(Scene* scene);

	virtual void render();
	virtual void update(float seconds_elapsed);
	virtual void initStage() {};

	Scene* getScene();
};

class PlayStage : public Stage {

private:
	Entity* root;

public:
	PlayStage();
	void render() override;
	void update(float elapsed_time) override;


};
