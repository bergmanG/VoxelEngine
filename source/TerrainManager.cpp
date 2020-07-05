#include "TerrainManager.h"
#include <stdlib.h>
#include <algorithm>
#include "Game.h"

TerrainManager::TerrainManager() {
    for(int t = 0; t <map_size; t++) {
        for(int w = 0; w< map_size; w++) {
            for(int i = 0; i< map_size; i++) {
                int random = rand() % 3;
                Chunk * c = new Chunk;
                for(int x =0; x<16; x++) {
                    for(int y =0; y<16; y++) {
                        for(int z=0; z<16; z++) {
							if (w > 5) {
								//c->voxels.set(x,y,z,0);
							}
							else {
								switch (random) {
								case 0:
									//c->voxels.set(x, y, z, 0);
									//c->data[x][y][z] = rand() % 3 +1 ;
									break;
								case 1:
									//c->data[x][y][z] = 0;
									c->voxels.set(x, y, z, rand() % 4);
									break;
								case 2:
									//c->data[x][y][z] = 0;
									c->voxels.set(x, y, z, rand() % 4);
									break;
								}
							}
                        }
                    }
                }
                c->world_pos = glm::vec3(16*t,16*w,16*i);
                chunk_map[t][w][i] = c;
                c->updateVao();
                Game::renderer->insertRenderList(chunk_map[t][w][i]);
            }
        }
    }
}
Chunk * TerrainManager::findChunk(glm::vec3 pos) {
	if (pos.x >= map_size * 16 ||
		pos.y >= map_size * 16 ||
		pos.z >= map_size * 16 ||
		pos.x < 0 || pos.y < 0 || pos.z < 0)
	{
		return 0;
	}

	return chunk_map[int(pos.x/16)][int(pos.y/16)][int(pos.z/16)];
}
int TerrainManager::findBlock(glm::vec3 pos) {
	Chunk* c = findChunk(pos);
	if (c == 0) {
		return -1;
	}
	glm::ivec3 trunc_pos(floor(pos.x), floor(pos.y), floor(pos.z));
	return c->voxels.get(trunc_pos.x - (int)c->world_pos.x,trunc_pos.y - (int)c->world_pos.y,trunc_pos.z - (int)c->world_pos.z);
}
bool TerrainManager::removeBlock(glm::vec3 pos) {
	Chunk* c = findChunk(pos);
	if (c == 0) {
		return false;
	}
	glm::ivec3 trunc_pos(floor(pos.x), floor(pos.y), floor(pos.z));
	c->voxels.set(trunc_pos.x - (int)c->world_pos.x, trunc_pos.y - (int)c->world_pos.y, trunc_pos.z - (int)c->world_pos.z, 0);
	c->updateVao();
	return true;
}
bool TerrainManager::placeBlock(glm::vec3 pos, VoxelFace face, int type) {
	glm::ivec3 trunc_pos(floor(pos.x), floor(pos.y), floor(pos.z));
	glm::ivec3 newpos;
	switch (face) {
	case BOT:
		newpos = trunc_pos + glm::ivec3(0,-1,0);
		break;
	case TOP:
		newpos = trunc_pos + glm::ivec3(0, 1, 0);
		break;
	case WEST:
		newpos = trunc_pos + glm::ivec3(1, 0, 0);
		break;
	case EAST:
		newpos = trunc_pos + glm::ivec3(-1, 0, 0);
		break;
	case FRONT:
		newpos = trunc_pos + glm::ivec3(0, 0, -1);
		break;
	case BACK:
		newpos = trunc_pos + glm::ivec3(0, 0, 1);
		break;
	default:
		break;
	}
	Chunk* c = findChunk(newpos);
	if (c == 0) {
		return false;
	}
	if (findBlock(newpos) != 0) {
		return false;
	}
	c->voxels.set(newpos.x - (int)c->world_pos.x, newpos.y - (int)c->world_pos.y, newpos.z - (int)c->world_pos.z, type);
	c->updateVao();
	return true;
}