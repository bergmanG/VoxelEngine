#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <algorithm>
enum VoxelFace {
	BOT,
	TOP,
	WEST,
	EAST,
	FRONT,
	BACK
};
class VoxelContainer{
	template <typename T>
	struct VoxelArray
	{
		T* data = 0;
		bool empty = true;
		uint32_t width, height, depth;
	public:
		VoxelArray(uint32_t width_, uint32_t height_, uint32_t depth_) { width = width_; height = height_; depth = depth_; };
		VoxelArray(uint32_t width_, uint32_t height_, uint32_t depth_, T value) {
			width = width_; height = height_; depth = depth_;
			data = new T[width * height * depth];
			std::fill(data,data+width*height*depth,value);
			empty = false;
		};
		void allocate() { delete[] data; data = new T[width * height * depth]; empty = false;};
		void deallocate() { delete[] data; empty = true; };
		T get(uint32_t first_index, uint32_t sencond_index, uint32_t third_index) {
			return empty ? 0 : data[first_index + sencond_index * width + third_index * (height*depth)];
		};
		void set(uint32_t first_index, uint32_t sencond_index, uint32_t third_index, T value) {
			if (!empty) {
				data[first_index + sencond_index * width + third_index * (height * depth)] = value;
			}
			else {
				data = new T[width * height * depth]{0};
				empty = false;
				data[first_index + sencond_index * width + third_index * (height * depth)] = value;
			}
		};
		~VoxelArray() { delete[] data; };
	};
	typedef VoxelArray<uint8_t>* FaceMaskArray;
	uint32_t voxel_count = 0;
	GLuint vbo_hdls[2];
	FaceMaskArray genMask();

	static const int block_colors[9][3];
	static const uint8_t face_masks[6];
	static uint32_t packVertex(glm::ivec3 offset, glm::bvec3 dist, uint32_t side);
	static uint32_t getPackedColor(int block_type);
	static void bufferPositionArray(glm::vec3 offset, uint32_t index, uint8_t mask, GLuint* vert_info);
	static int bufferColorArray(uint32_t index, int block_type, uint8_t mask, GLuint* color_buffer);
public:
	GLuint vao_hdl = 0;
	int num_vertices = 0;
	glm::vec3 world_pos;
	uint32_t width, height, depth;
	VoxelArray<int> voxels;
	void updateVao();
	VoxelContainer(uint32_t width_, uint32_t height_, uint32_t depth_) :voxels(width_, height_, depth_) {
		width = width_; height = height_; depth = depth_;
	};
	VoxelContainer(uint32_t width_, uint32_t height_, uint32_t depth_, int value):voxels(width_, height_, depth_, value) {
		width = width_; height = height_; depth = depth_;
	};
};
struct Chunk : VoxelContainer {
	Chunk():VoxelContainer(16,16,16) {};
};