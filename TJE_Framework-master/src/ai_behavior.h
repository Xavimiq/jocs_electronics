#pragma once
#include "framework.h"
//#include "extra/pathfinder/PathFinder.h"
//#include


enum eActions { SEARCH, ATTACK, FLEE};

//class WayPoint : public DijkstraNode
//{

//};

class AIBehavior {

public:

	void setModel(Matrix44* m) { model = m; };
	void update(float seconds_elapsed);

	bool canSeeTarget();
	void searchTarget(float seconds_elapsed);
	//void lookAtTarget(Vector3& target, float seconds_elapsed);
	//bool hasAmmo()
	//void shoot();

private:
	eActions state = SEARCH;
	Matrix44* model;

	float position_z_pos = 5.f;
	float position_z_neg = -5.f;

	bool direction = true;

	bool walk_forwards = true;

	//std::vector<WayPoint> path;

	int wayPointIndex = 0;
	float timer = 0.f;

};

