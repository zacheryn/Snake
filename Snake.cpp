// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Snake.hpp"
#include <iostream>

int main() {
    bool cont = true;
    while (cont) {
        unsigned char temp = _getch();
        if (temp == KEY_ARROW_CHAR1) {
            switch (_getch()) {
            case KEY_ARROW_UP:
                std::cout << "Pressed up arrow\n" << std::endl;
                break;
            case KEY_ARROW_DOWN:
                std::cout << "Pressed up down\n" << std::endl;
                break;
            case KEY_ARROW_LEFT:
                std::cout << "Pressed up left\n" << std::endl;
                break;
            case KEY_ARROW_RIGHT:
                std::cout << "Pressed up right\n" << std::endl;
                break;
            default:
                std::cout << "Pressed unknown key\n" << std::endl;
                break;
            }
        }
        else if (temp == KEY_ESCAPE) {
            std::cout << "Exiting...\n" << std::endl;
            cont = false;
        }
        else {
            std::cout << "Pressed unknown key\n" << std::endl;
        }
    }
}
