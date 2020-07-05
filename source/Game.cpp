#include "Game.h"
#include <iostream>

Camera* Game::camera;
Renderer* Game::renderer;
TerrainManager* Game::terrain_mgr;
InputHandler* Game::inpt_hdlr;
GLSLProgram* Game::glsl_prog;
PhysicsEngine* Game::phy_eng;
sf::RenderWindow* Game::window;
VoxelContainer* Game::player;

void Game::start() {

	setupWindow();

	camera = new Camera();
	renderer = new Renderer();
	terrain_mgr = new TerrainManager();
	inpt_hdlr = new InputHandler();
	glsl_prog = new GLSLProgram();
	phy_eng = new PhysicsEngine();
	player = new VoxelContainer(2,2,2,5);


	setupShader();

	camera->setPersperctive(45.0f, 1500 / 700, 0.1, 200.0);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	player->world_pos = glm::vec3(6, 96, 7);
	player->updateVao();
	renderer->insertRenderList(player);
	while (game_loop()) {
	}
}
int Game::game_loop() {
	if (!inpt_hdlr->update()) {
		return 0;
	}
	glsl_prog->setUniform("MVP", camera->getMvp());
	glsl_prog->setUniform("Model", camera->getModel());
	glsl_prog->setUniform("LightPos", glm::vec3(0,0,0));

	renderer->draw();
	window->display();

	return 1;
}
void Game::setupWindow() {
	int width, height;
	width = 1500;
	height = 700;

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	Game::window = new sf::RenderWindow(sf::VideoMode(width, height), "Engine", sf::Style::Default, settings);

	Game::window->setFramerateLimit(60);

	Game::window->setActive(true);

	glewInit();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);

	Game::window->setMouseCursorVisible(false);
}
void Game::setupShader() {
	glsl_prog->compileShaderFromFile("basic.vert", GLSLShader::VERTEX);
	std::cout << "vertex shader log:\n";
	glsl_prog->printLog();

	glsl_prog->compileShaderFromFile("basic.frag", GLSLShader::FRAGMENT);
	std::cout << "Frag shader log:\n";
	glsl_prog->printLog();


	glsl_prog->link();
	std::cout << "Linking shader log:\n";
	glsl_prog->printLog();
	glsl_prog->use();
}
