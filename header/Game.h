#ifndef GAME_H
#define GAME_H
#include "Camera.h"
#include "Renderer.h"
#include "TerrainManager.h"
#include "InputHandler.h"
#include "GLSLProgram.h"
#include "PhysicsEngine.h"

class Game {
public:
	static sf::RenderWindow* window;
	static Camera* camera;
	static PhysicsEngine* phy_eng;
	static Renderer* renderer;
	static TerrainManager* terrain_mgr;
	static InputHandler* inpt_hdlr;
	static GLSLProgram* glsl_prog;
	static VoxelContainer* player;

	Game() {};
    void start();
    void setupWindow();
    void setupShader();
    int game_loop();

private:
};

#endif // GAME_H
