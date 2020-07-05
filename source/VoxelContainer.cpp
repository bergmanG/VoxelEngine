#include "VoxelContainer.h"
#include "Game.h"
const int VoxelContainer::block_colors[9][3] = {
	{0,0,0},
	{0,130,0},
	{0,0,130},
	{130,0,0},
	{255,133,0},
	{180,180,0},
	{0,200,200},
	{153,102,51},
	{255,255,255}
};
const uint8_t VoxelContainer::face_masks[6] = { 0x1,0x2,0x4,0x8,0x10,0x20 };
uint32_t VoxelContainer::packVertex(glm::ivec3 offset, glm::bvec3 dist, uint32_t side) {
	unsigned int x = offset.x + dist.x;
	unsigned int y = offset.y + dist.y;
	unsigned int z = offset.z + dist.z;

	unsigned int s = side;

	return z | (y << 5) | (x << 10) | (side << 15);
}
uint32_t VoxelContainer::getPackedColor(int block_type) {
	unsigned int r, g, b;
	r = block_colors[block_type][0];
	g = block_colors[block_type][1];
	b = block_colors[block_type][2];
	return (r | (g << 8) | (b << 16));
}
void VoxelContainer::bufferPositionArray(glm::vec3 offset, uint32_t index, uint8_t mask, GLuint* vert_info) {
	//BOT
	if (mask & face_masks[BOT]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 0), 0);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 0), 0);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 1), 0);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 1), 0);
		index++;
	}
	//TOP
	if (mask & face_masks[TOP]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 0), 1);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 1), 1);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 1), 1);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 0), 1);
		index++;
	}
	//WEST
	if (mask & face_masks[WEST]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 0), 2);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 0), 2);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 1), 2);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 1), 2);
		index++;
	}
	//EAST
	if (mask & face_masks[EAST]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 0), 3);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 1), 3);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 1), 3);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 0), 3);
		index++;
	}
	//FRENTE
	if (mask & face_masks[FRONT]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 0), 4);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 0), 4);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 0), 4);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 0), 4);
		index++;
	}
	//TRAS
	if (mask & face_masks[BACK]) {
		vert_info[0 + index * 4] = packVertex(offset, glm::bvec3(0, 0, 1), 5);
		vert_info[1 + index * 4] = packVertex(offset, glm::bvec3(1, 0, 1), 5);
		vert_info[2 + index * 4] = packVertex(offset, glm::bvec3(1, 1, 1), 5);
		vert_info[3 + index * 4] = packVertex(offset, glm::bvec3(0, 1, 1), 5);
	}
}
int VoxelContainer::bufferColorArray(uint32_t index, int block_type, uint8_t mask, GLuint* color_buffer) {
	int num_faces = 0;
	for (int j = 0; j < 6; j++) {
		if (mask & face_masks[j]) { num_faces++; }
	}

	unsigned int packed_color = getPackedColor(block_type);

	for (int i = 0; i < 4 * num_faces; i++) {
		color_buffer[i + 4 * index] = packed_color;
	}
	return num_faces;
}
void VoxelContainer::updateVao() {

	if (vao_hdl == 0) {
		glGenVertexArrays(1, &vao_hdl);
		glGenBuffers(2, &vbo_hdls[0]);

		glBindVertexArray(vao_hdl);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Game::renderer->ibo_hdl);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_hdls[0]);

		glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0,
			(GLubyte*)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_hdls[1]);

		glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0,
			(GLubyte*)NULL);

	}

	int face_count = 0;
	FaceMaskArray mask = genMask();

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				for (int w = 0; w < 6; w++) {
					if (mask->get(x,y,z) & face_masks[w] && voxels.get(x, y, z) != 0) {
						face_count++;
					}
				}
			}
		}
	}
	int vert_buf_size = face_count * 4;

	GLuint* vert_info = new GLuint[vert_buf_size];
	GLuint* color = new GLuint[vert_buf_size];

	int face_index = 0;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				int block_type = voxels.get(x, y, z);
				if (block_type != 0) {
					glm::ivec3 offset(x, y, z);
					bufferPositionArray(offset, face_index, mask->get(x,y,z), vert_info);
					int num_faces = bufferColorArray(face_index, block_type, mask->get(x, y, z), color);
					face_index += num_faces;
				}
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo_hdls[0]);
	glBufferData(GL_ARRAY_BUFFER, vert_buf_size * sizeof(unsigned int), vert_info,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_hdls[1]);
	glBufferData(GL_ARRAY_BUFFER, vert_buf_size * sizeof(unsigned int), color,
		GL_STATIC_DRAW);
	num_vertices = face_count * 2 * 3;
	delete mask;
	delete[] vert_info;
	delete[] color;
}
VoxelContainer::FaceMaskArray VoxelContainer::genMask() {

	FaceMaskArray mask = new VoxelArray<uint8_t>(width, height, depth, 0x0);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				if (x == 0 || x == width - 1 ||
					y == 0 || y == height - 1 ||
					z == 0 || z == depth - 1)
				{
					mask->set(x, y, z, 0x3F);
				}
				if (voxels.get(x, y, z) == 0) {
					if (x < depth - 1) { mask->set(x + 1, y, z, face_masks[EAST]); }
					if (y < height - 1) { mask->set(x, y + 1, z, mask->get(x, y + 1, z) | face_masks[BOT]); }
					if (z < depth - 1) { mask->set(x, y, z + 1, mask->get(x, y, z + 1) | face_masks[FRONT]); }
					if (x > 0) { mask->set(x - 1, y, z, mask->get(x - 1, y, z) | face_masks[WEST]); }
					if (y > 0) { mask->set(x, y - 1, z, mask->get(x, y - 1, z) | face_masks[TOP]); }
					if (z > 0) { mask->set(x, y, z - 1, mask->get(x, y, z - 1) | face_masks[BACK]); }
				}
			}
		}
	}
	return mask;
}