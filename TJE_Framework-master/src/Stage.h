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
	EntityPlayer* my_player;


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
	//Entity root;
	
	

public:
	bool showInstruccions = true;
	//World* my_world;

	PlayStage();
	virtual void render() override;
	virtual void update(float elapsed_time) override;

	Entity guiElements;
	EntityGUIelement* instruccionsButton;

	EntityEnemy* my_enemy_1;
	EntityEnemy* my_enemy_2;
	
	EntityPrisoner* prisoner_1;

	EntityDoor* Door0;
	EntityKey* key0;

	EntityDoor* Door1;
	EntityKey* key1;

};

class MenuStage : public Stage {


public:
	float ang;

	Entity guiElements;

	EntityGUIelement* titleButton = nullptr;
	EntityGUIelement* playButton = nullptr;
	EntityGUIelement* exitButton = nullptr;

	EntityMesh* floorplane;


	Vector3 eye;
	Vector3 center;


	MenuStage();
	void render() override;
	void update(float elapsed_time) override;


};



class WinStage : public Stage {


public:
	float ang;

	Entity guiElements;

	EntityGUIelement* titleButton = nullptr;
	//EntityGUIelement* playButton = nullptr;
	//EntityGUIelement* exitButton = nullptr;

	//EntityMesh* floorplane;


	Vector3 eye;
	Vector3 center;


	WinStage();
	void render() override;
	void update(float elapsed_time) override;


};