#include <cstdlib>
#include <algorithm>
#include "PhysicsEngine.h"
#include "Game.h"
int sign(float);
float smallest(float, float, float);
RayPickResult PhysicsEngine::rayPick(glm::vec3 origin, glm::vec3 dir, int range) {
	bool success = true;
	glm::vec3 normal_dir = glm::normalize(dir);

	float stepX = sign(normal_dir.x);
	float stepY = sign(normal_dir.y);
	float stepZ = sign(normal_dir.z);

	float tDeltaX = stepX / normal_dir.x;
	float tDeltaY = stepY / normal_dir.y;
	float tDeltaZ = stepZ / normal_dir.z;

	float xDist = stepX > 0 ? ceil(origin.x) - origin.x : origin.x - floor(origin.x);
	float tMaxX = xDist / fabsf(normal_dir.x);

	float yDist = stepY > 0 ? ceil(origin.y) - origin.y : origin.y - floor(origin.y);
	float tMaxY = yDist / fabsf(normal_dir.y);

	float zDist = stepZ > 0 ? ceil(origin.z) - origin.z : origin.z - floor(origin.z);
	float tMaxZ = zDist / fabsf(normal_dir.z);

	AXIS last = Y;
	int count = 0;
	glm::ivec3 trunc_origin(floor(origin.x), floor(origin.y), floor(origin.z));

	if (Game::terrain_mgr->findBlock(origin) > 0) {
		return RayPickResult(success,trunc_origin,TOP);
	}
	int x = trunc_origin.x;
	int y = trunc_origin.y;
	int z = trunc_origin.z;

	while (count <= range) {
		float min = smallest(tMaxX, tMaxY, tMaxZ);
		if (min == tMaxX) {
			tMaxX += tDeltaX;
			x += stepX;
			last = X;
			count++;
		}
		else if (min == tMaxY) {
			tMaxY += tDeltaY;
			y += stepY;
			last = Y;
			count++;
		}
		else
		{
			tMaxZ += tDeltaZ;
			z += stepZ;
			last = Z;
			count++;
		}
		if (Game::terrain_mgr->findBlock(glm::vec3(x, y, z)) > 0) {
			if (last == X) {
				if (stepX>0) {return RayPickResult(success, glm::vec3(x, y, z),EAST);}
				else { return RayPickResult(success, glm::vec3(x, y, z), WEST);}
			}
			else if (last == Y) {
				if(stepY>0){return RayPickResult(success, glm::vec3(x, y, z), BOT);}
				else{ return RayPickResult(success, glm::vec3(x, y, z), TOP);}
			}
			else {
				if (stepZ > 0) { return RayPickResult(success, glm::vec3(x, y, z), FRONT);
				}
				else { return RayPickResult(success, glm::vec3(x, y, z), BACK);
				}
			}

		}
	}
	success = false;
	return RayPickResult(success, glm::vec3(0, 0, 0), EAST);
}
CollisionResult PhysicsEngine::checkCollision(VoxelContainer*e, glm::vec3 target_pos) {
	for (float x = floor(target_pos.x); x <= target_pos.x + e->width; x++) {
		for (float y = floor(target_pos.y); y <= target_pos.y + e->height; y++) {
			for (float z = floor(target_pos.z); z <= target_pos.z + e->depth; z++) {
				if (Game::terrain_mgr->findBlock(glm::vec3(x,y,z))>0) {
					return CollisionResult(true,glm::vec3(x,y,z));
				}
			}
		}
	}
 return CollisionResult(false, glm::vec3(0, 0, 0));
}
void PhysicsEngine::moveEntity(VoxelContainer* e, glm::vec3 dir) {
	float speed = 0.1;
	CollisionResult cr = checkCollision(e, e->world_pos + speed * dir);
	if (cr.collided) {
		if (dir.x > 0) {
			e->world_pos.x = cr.target.x - e->width - 0.0001;
		}
		else if (dir.x <0) {
			e->world_pos.x = cr.target.x + 1;
		}
		if (dir.y > 0) {
			e->world_pos.y = cr.target.y - e->height - 0.0001;
		}
		else if (dir.y < 0) {
			e->world_pos.y = cr.target.y + 1;
		}
		if (dir.z > 0) {
			e->world_pos.z = cr.target.z - e->depth - 0.0001;
		}
		else if (dir.z < 0) {
			e->world_pos.z = cr.target.z + 1;
		}
	}
	else {
		e->world_pos = e->world_pos + speed * dir;
	}
}
int sign(float x) {
	return (x > 0) - (x < 0);
}
float smallest(float x, float y, float z) {
	return std::min(std::min(x, y), z);
}
