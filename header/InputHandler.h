#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <SFML/Graphics.hpp>

class InputHandler
{
    public:
		InputHandler() {};
        int update();

    private:
        float walking_speed = 0.2;
        float sensibility = 0.02;
        sf::Vector2i mouse_pos = sf::Mouse::getPosition();
		int selected_type = 8;
        int win_width = 1500;
        int win_height= 700;
        bool window_focus = true;
		bool mouse_pressed_handled = false;
};

#endif // INPUTHANDLER_H
