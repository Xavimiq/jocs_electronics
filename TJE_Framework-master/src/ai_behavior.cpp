#include "ai_behavior.h"

#include "World.h"
#include "mesh.h"
#include "mesh.h"
#include <iostream>

void AIBehavior::update(float seconds_elapsed)
{
	if (state == SEARCH)
	{
		searchTarget(seconds_elapsed);

		if (canSeeTarget()) {
			std::cout << "target spotted!" << std::endl;
			state = ATTACK;
		}
		else {
			//searchTarget(seconds_elapsed);
		}

	}
	else if (state == ATTACK)
	{
		//World* world = World::get_instance();
		//Vector3 target = world->my_player->model.getTranslation();

		//lookAtTarget(target, seconds_elapsed);

		if (!canSeeTarget()) {
			std::cout << "target missed!" << std::endl;
			state = SEARCH;
		}
	}
	else if (state == FLEE)
	{
		state = SEARCH;
	}
	//std::cout << state << std::endl;
	position_z_pos += (float)seconds_elapsed * 50.0f;
	
	position_z_neg += (float)seconds_elapsed * 50.0f;
}

bool AIBehavior::canSeeTarget()
{
	World* world = World::get_instance();
	Matrix44 target = world->my_player->model;
	//world->checkLineOfSight(model&, );
	return false;

}

void AIBehavior::searchTarget(float seconds_elapsed)
{
	if (model->getTranslation().z < 400.f && direction) {
		model->setTranslation(0, 0, position_z_pos);
	}
	else {
		if (model->getTranslation().z >= 400.f && model->getTranslation().z <= 100.f) {
			// Cambio de dirección desde 400 a 100
			position_z_pos = -position_z_pos;
			model->setTranslation(0, 0, position_z_pos);
		}
		else if (model->getTranslation().z > 100.f && model->getTranslation().z <= 400.f) {
			// Cambio de dirección desde 100 a 400
			position_z_pos = -position_z_pos;
			model->setTranslation(0, 0, position_z_neg);
		}
	}
}

//void AIBehavior::searchTarget(float seconds_elapsed) {
//	if (path.size() && path.size() == World::get_instance()->waypoints.size())
//	{
//		Vector3 origin = model->getTranslation();
//		origin.y = 0.f;
//		Vector3 target = Vector3(path[wayPointIndex].postion.x, 0.f, path[wayPointIndex].position.z);
//
//		float toTargetLength = (target - origin).length();
//
//		lookAtTarget(target, seconds_elapsed);
//		model->translate(0.f, 0.f, seconds_elapsed * 2.f);
//
//		if (toTargetLength < 0.1f)
//		{
//			if (walk_forwards && wayPointIndex + 1 == path.size()) {
//				walk_forwards = false;
//			}
//
//			if (!walk_forwards && wayPointIndex - 1 < 0) {
//				walk_forwards = true;
//			}
//
//			if (walk_forwards) {
//				wayPointIndex++;
//			}
//			else {
//				wayPointIndex--;
//			}
//		}
//	}
//	else
//	{
//		if (World::get_instance().waypoints.size() < 2)
//		{
//			return;
//		}
//		path.clear();
//
//		const std::vector<Vector3>& points = World::get_instance()->waypoints;
//		std::vector<WayPoint> wps;
//		wps.resize(points.size());
//
//		for (int i = 0; i < points.size() - 1; ++i)
//		{
//			wps[i].position = points[i];
//			wps[i + 1].position = points[i + 1];
//			//wps[i].addLink(&wps[i + 1],)
//		}
//	}
//}