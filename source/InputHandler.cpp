#include "InputHandler.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Game.h"

int InputHandler::update() {

	sf::Event event;
	while (Game::window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			return 0;
			break;
		case sf::Event::Resized:
			glViewport(0, 0, event.size.width, event.size.height);
			break;
		case sf::Event::LostFocus:
			window_focus = false;
			break;
		case sf::Event::GainedFocus:
			window_focus = true;
			break;
		case sf::Event::MouseButtonPressed:
			if (!mouse_pressed_handled) {
				mouse_pressed_handled = true;
				RayPickResult r = PhysicsEngine::rayPick(Game::camera->eye_pos, Game::camera->view, 80);
				if (r.success) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						Game::terrain_mgr->removeBlock(r.target);
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						Game::terrain_mgr->placeBlock(r.target, r.face, selected_type);
					}
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			mouse_pressed_handled = false;
			break;
		default:
			break;
		}
	}

	sf::Vector2i new_mouse_pos = sf::Mouse::getPosition();

	if (window_focus) {
		sf::Mouse::setPosition(sf::Vector2i(win_width / 2, win_height / 2), *Game::window);
	}

	mouse_pos = sf::Mouse::getPosition();

	Game::camera->look(sensibility * (mouse_pos.x - new_mouse_pos.x), sensibility * (mouse_pos.y - new_mouse_pos.y));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		Game::camera->walk(glm::normalize(Game::camera->view), walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		Game::camera->walk(glm::normalize(Game::camera->view), -walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		Game::camera->walk(glm::normalize(glm::cross(Game::camera->view, Game::camera->up)), -walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		Game::camera->walk(glm::normalize(glm::cross(Game::camera->view, Game::camera->up)), walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		Game::camera->walk(glm::vec3(0, 1, 0), walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		Game::camera->walk(glm::vec3(0, -1, 0), walking_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		selected_type = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		selected_type = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		selected_type = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
		selected_type = 4;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
		selected_type = 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
		selected_type = 6;
	}
 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(0, 0, 1));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(0, 0, -1));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(-1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(0, 1, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		PhysicsEngine::moveEntity(Game::player, glm::vec3(0, -1, 0));
	}

	return 1;
}
