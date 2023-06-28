#pragma once
#include "framework.h"
//#include "Entity.h"
#include "extra/pathfinder/PathFinder.h"
#include "extra/pathfinder/Dijkstra.h"
#include "anim_states.h"
//#include

class EntityPlayer;

enum eActions { SEARCH, ATTACK, FLEE};

class WayPoint : public DijkstraNode
{
public:

	WayPoint(){}

	WayPoint(const Vector3& position)
	{
		this->position = position;
	}

	void addLink(WayPoint* waypoint, float distance)
	{
		addChild(waypoint, distance);
		waypoint->addChild(this, distance);
	}

	Vector3 position;


};

class AIBehavior {

public:

	//EntityEnemy* enemy;
	std::vector<Vector3> waypoints;
	EntityPlayer* my_player;
	AnimState anim_state;

	void setModel(Matrix44* m) { model = m; };
	void update(float seconds_elapsed);

	bool canSeeTarget();
	void searchTarget(float seconds_elapsed);
	void lookAtTarget(const Vector3& target, float seconds_elapsed);

	
	void addWayPoints(Vector3 vector) { waypoints.push_back(vector); }


private:
	eActions state = SEARCH;
	Matrix44* model;



	//float position_z_pos = 5.f;
	//float position_z_neg = -5.f;

	//bool direction = true;

	bool walk_forwards = true;

	std::vector<WayPoint> path;

	int wayPointIndex = 0;
	float timer = 0.f;

};

