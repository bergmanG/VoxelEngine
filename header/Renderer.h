#ifndef RENDERER_H
#define RENDERER_H
#include <list>
#include <GL/glew.h>
#include "VoxelContainer.h"
class Renderer
{
public:

	Renderer();
	void draw();
	void insertRenderList(VoxelContainer* r);
	GLuint ibo_hdl;
private:
	std::list<VoxelContainer*> terrain_vao_hdls;
};

#endif // RENDERER_H
