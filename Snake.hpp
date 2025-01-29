#pragma once

#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <conio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <time.h>
#include "Values.hpp"
#include <vector>

// (0, 0) is bottom left of board

// Runs the game Snake through stdin
class Snake {
private:

	// Holds the coordinates for anything on the board
	//
	// They are stored as unsigned chars to save space
	struct Coord {
		unsigned char x;
		unsigned char y;

		Coord() :
		x{ 0 }, y{ 0 } {}

		Coord(unsigned char _x, unsigned char _y) :
		x{ _x }, y{ _y } {}
	};


	// Determine the direction of the latest input
	enum class Direction : unsigned char {
		Up,
		Down,
		Left,
		Right
	};


	std::string board;					// A 15 x 15 board for the game plus a border making the string 17 x 17
	std::vector<Coord> snake;			// Holds the locations of the snake (index 0 is always head)
	Coord food;							// Holds the current location of the food
	std::unique_ptr<std::mt19937> rng;	// The rng for determining the next food location


	// Print the current state of the board
	void print_board() {
		std::cout << "\n\n" << board << std::endl;
	}

public:

	// Basic constructor for setting up the initial conditions of the board
	Snake() : board{ INITIAL_BOARD }, rng{ new std::mt19937(time(NULL)) } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
	}


	// Starts a game of snake
	void play() {

	}

};

#endif
