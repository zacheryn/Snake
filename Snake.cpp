// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Snake.hpp"

int main(int argc, char* argv[]) {
    std::string path(argv[0]);
    Snake s(path.substr(0, path.find_last_of("\\/")) + "\\Sounds\\");

    s.play();

    std::cout << "Press any key to exit..." << std::flush;
    _getch();

    return 0;
}
