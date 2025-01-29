#pragma once

#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <iostream>
#include <string>
#include <conio.h>

// Runs the game Snake through stdin
class Snake {
private:

	// 
	struct Coord {
		unsigned char x;
		unsigned char y;

		Coord() :
		x{ 0 }, y{ 0 } {}

		Coord(unsigned char _x, unsigned char _y) :
		x{ _x }, y{ _y } {}
	};

	std::string board;
	unsigned char length;

public:

};

#endif
