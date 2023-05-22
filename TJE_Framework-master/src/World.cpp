#include "World.h"
#include "framework.h"

World::World() {

}

bool World::checkLineOfSight(const Matrix44& obs, const Matrix44& target) {


Vector3 front = normalize(obs.frontVector());
Vector3 toTarget = normalize(target.getTranslation() - obs.getTranslation());

	if (toTarget.dot(front) > 0.5) {

		for (auto e : root.children) {
			EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
			if (!ec)

				ec->mesh->testRayCollision(
					ec->model,
					ray
			)

		}
		return true;
	}
}
