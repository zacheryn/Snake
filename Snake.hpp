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
		size_t x;
		size_t y;

		Coord() :
		x{ 0 }, y{ 0 } {}

		Coord(size_t _x, size_t _y) :
		x{ _x }, y{ _y } {}
	};


	// Determine the direction of the latest input
	enum class Direction : uint8_t {
		Up,
		Down,
		Left,
		Right
	};


	// A 15 x 15 board for the game plus a border making the string 17 x 17
	std::string board;

	// Holds the locations of the snake (index 0 is always head)
	std::vector<Coord> snake;

	// Holds the current location of the food
	Coord food;

	// The rng for determining the next food location
	std::unique_ptr<std::mt19937> rng;

	// The size of the board
	constexpr static size_t dim = 17;

	// The maximum length of the snake. When reached, you win.
	constexpr static size_t MAX_LENGTH = 225;


	// Print the current state of the board
	void print_board() {
		std::cout << "\n\n" << board << std::endl;
	}


	// Set the board to INITIAL BOARD
	void reset_board() {
		board.assign(INITIAL_BOARD);
	}


	// Place the Snake on the board
	void place_snake() {
		for (const Coord c : snake) {
			board[(1 + c.y) * 17 + c.x + 1] = BODY;
		}
		board[(1 + snake.front().y) * 17 + snake.front().x + 1] = HEAD;
	}


	// Updates the snake's position given a specific input
	void update_snake(Direction dir, bool ate) {

	}


	// Randomly generate the next food location in a valid space
	Coord generate_next_food() {
		std::vector<Coord> available;
		for (size_t y = 0; y < 15; ++y) {
			for (size_t x = 0; x < 15; ++x) {
				if (board[(1 + y) * 17 + x + 1] == ' ')
					available.emplace_back(x, y);
			}
		}

		if (available.empty()) return Coord(50, 50);

		std::uniform_int_distribution<size_t> dist(0, available.size() - 1);

		return available[dist(rng)];
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
