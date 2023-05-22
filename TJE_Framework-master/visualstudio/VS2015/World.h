#ifndef WORLD_H
#define WORLD_H

#include "../../src/framework.h"

class World
{
	World::World();
	bool World::checkLineOfSight(const Matrix44& obs, const Matrix44& target);
};


#endif
