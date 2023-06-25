#ifndef AI_H
#define AI_H



#include "ai_behavior.h"
#include "World.h"
#include "mesh.h"
#include <iostream>
#include "Entity.h"

void AIBehavior::update(float seconds_elapsed)
{
	if (state == SEARCH)
	{
		searchTarget(seconds_elapsed);

		if (canSeeTarget()) {
			std::cout << "target spotted!" << std::endl;
			state = ATTACK;
		}

	}
	else if (state == ATTACK)
	{
		//World* world = World::get_instance();
		Vector3 target = my_player->model.getTranslation();

		lookAtTarget(target, seconds_elapsed);

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

}

bool AIBehavior::canSeeTarget()
{
	//World* world = World::get_instance();
	Matrix44 target = my_player->model;
	
	Vector3 enemyFront = model->frontVector(); 

	

	Vector3 toTarget = target.getTranslation() - model->getTranslation(); 
	float distance = toTarget.length();
	toTarget = toTarget.normalize();

	Vector3 ray_origin = model->getTranslation();
	Vector3 ray_direction = toTarget;

	if (toTarget.dot(enemyFront) > 0.5)
	{
		for (auto e : World::get_instance()->root.children)
		{
			EntityMesh* em = dynamic_cast<EntityMesh*>(e);
			//if (!ec) continue;
			if (em->isCollider = true)
			{
				if (em->mesh->testRayCollision(
					em->model,
					ray_origin,
					ray_direction,
					Vector3(),
					Vector3(),
					distance
				)) {
					return false; //NO COLISIONA
				}
			}

		}
		return true;
	}
	return false;

}


void AIBehavior::searchTarget(float seconds_elapsed) {
	if (path.size() && path.size() ==  waypoints.size())
	{
		Vector3 origin = model->getTranslation();
		origin.y = 0.f;
		Vector3 target = Vector3(path[wayPointIndex].position.x, 0.f, path[wayPointIndex].position.z);

		float toTargetLength = (target - origin).length();

		lookAtTarget(target, seconds_elapsed);
		model->translate(0.f, 0.f, seconds_elapsed * 14.f);

		if (toTargetLength < 10.f)
		{
			if (walk_forwards && wayPointIndex + 1 == path.size()) {
				walk_forwards = false;
			}

			if (!walk_forwards && wayPointIndex - 1 < 0) {
				walk_forwards = true;
			}

			if (walk_forwards) {
				wayPointIndex++;
			}
			else {
				wayPointIndex--;
			}
		}
	}
	else
	{
		if (waypoints.size() < 2)
		{
			return;
		}
		path.clear();

		const std::vector<Vector3>& points = waypoints;
		std::vector<WayPoint> wps;
		wps.resize(points.size());

		for (int i = 0; i < points.size() - 1; ++i)
		{
			wps[i].position = points[i];
			wps[i + 1].position = points[i + 1];
			wps[i].addLink(&wps[i + 1], (wps[i].position - wps[i + 1].position).length());
		}

		PathFinder<WayPoint> p;
		std::vector<WayPoint*> solution;

		p.setStart(wps[0]);
		p.setGoal(wps[wps.size() - 1]);

		bool r = p.findPath<Dijkstra>(solution);

		if (r)
		{
			std::cout << "Solution (" << solution.size() - 1 << " steps) :" << std::endl;
			for (const auto& wp : solution) {
				std::cout << wp->position.x << ", " << wp->position.y << ", " << wp->position.z << std::endl;
				path.push_back(wp->position);
			}
			std::cout << std::endl;
		}
		else {
			std::cerr << "No path was found, sorry. " << std::endl;
		}
	}
}

void AIBehavior::lookAtTarget(const Vector3& target, float seconds_elapsed) {
	
	Vector3 origin = model->getTranslation();
	origin.y = 0;
	float delta_yaw = model->getYawRotationToAimTo(target);
	model->rotate(delta_yaw * seconds_elapsed * 4, Vector3(0.f, 1.f, 0.f));

}
#endif
