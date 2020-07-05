#include "Renderer.h"
#include "Game.h"
#include <iostream>

Renderer::Renderer() {
	int ibo_buf_size = 16 * 16 * 16 * 6 * 2 * 3; /// blocos*faces*triangulos*indices
	GLuint* ibo_buf = new GLuint[ibo_buf_size];
	glGenBuffers(1, &ibo_hdl);

	for (int i = 0; i < ibo_buf_size / 6; i++) {
		ibo_buf[0 + 6 * i] = 0 + 4 * i;
		ibo_buf[1 + 6 * i] = 1 + 4 * i;
		ibo_buf[2 + 6 * i] = 3 + 4 * i;

		ibo_buf[3 + 6 * i] = 1 + 4 * i;
		ibo_buf[4 + 6 * i] = 2 + 4 * i;
		ibo_buf[5 + 6 * i] = 3 + 4 * i;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_hdl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_buf_size * sizeof(unsigned int), ibo_buf, GL_STATIC_DRAW);
	delete[] ibo_buf;
}
void Renderer::draw() {
	//glClearColor(1.0,1.0,1.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (std::list<VoxelContainer*>::iterator it = terrain_vao_hdls.begin(); it != terrain_vao_hdls.end(); ++it) {
		Game::glsl_prog->setUniform("world_pos", (*it)->world_pos);
		glBindVertexArray((*it)->vao_hdl);
		glDrawElements(GL_TRIANGLES, (*it)->num_vertices, GL_UNSIGNED_INT, 0);
	}
}
void Renderer::insertRenderList(VoxelContainer* r) {
	terrain_vao_hdls.push_front(r);
}