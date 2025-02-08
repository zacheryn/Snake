// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Snake.hpp"

int main() {
    Snake s;

    s.play();

    std::cout << "Press any key to exit..." << std::flush;
    _getch();

    return 0;
}
