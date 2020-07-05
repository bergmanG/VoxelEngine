#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H
#include <glm/glm.hpp>
#include "VoxelContainer.h"
enum AXIS {
	X,
	Y,
	Z
};
struct CollisionResult {
	bool collided;
	glm::vec3 target;
	CollisionResult(bool collided, glm::vec3 target) {
		this->collided = collided;
		this->target = target;
	};
};
struct RayPickResult {
	bool success;
	glm::vec3 target;
	VoxelFace face;
	RayPickResult(bool success, glm::vec3 target, VoxelFace face) {
		this->success = success;
		this->target = target;
		this->face = face;
	}
};
class PhysicsEngine {
public:
	PhysicsEngine() {};
	static RayPickResult rayPick(glm::vec3 origin, glm::vec3 dir, int range);
	static CollisionResult checkCollision(VoxelContainer* e, glm::vec3 target_pos);
	static void moveEntity(VoxelContainer* e, glm::vec3 dir);
	void tick();
};
#endif