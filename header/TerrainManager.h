#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H
#include <glm/glm.hpp>
#include "VoxelContainer.h"

class TerrainManager
{
    public:
        TerrainManager();
        Chunk *findChunk(glm::vec3 pos);
		int findBlock(glm::vec3 pos);
		bool removeBlock(glm::vec3 pos);
		bool placeBlock(glm::vec3 pos, VoxelFace face, int type);
	private:
		int map_size = 10;
        Chunk * chunk_map[10][10][10];
};

#endif // TERRAINMANAGER_H
