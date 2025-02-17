#pragma once

#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Audio.hpp"
#include <conio.h>
#include <deque>
#include <iostream>
#include <random>
#include <string>
#include <time.h>
#include "Values.hpp"

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

		Coord(const Coord& other) :
		x{ other.x }, y{ other.y } {}

		friend bool operator==(const Coord& lhs, const Coord& rhs) {
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}

		friend bool operator!=(const Coord& lhs, const Coord& rhs) {
			return lhs.x != rhs.x || lhs.y != rhs.y;
		}
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
	std::deque<Coord> snake;

	// Holds the current location of the food
	Coord food;

	// The rng for determining the next food location
	std::unique_ptr<std::mt19937> rng;

	// The size of the board
	constexpr static size_t dim = 17;

	// The maximum length of the snake. When reached, you win.
	constexpr static size_t MAX_LENGTH = 225;

	// True while the snake is still alive
	bool alive = true;


	// Print the current state of the board
	void print_board() {
		std::cout << "\n\n" << board << std::endl;
	}


	// Return a reference of the specified character
	char& grab_char(const Coord& c) {
		if (c.x > 14 || c.y > 14) throw std::invalid_argument("Both x and y must be less than or equal to 14");
		return board[(1 + c.y) * 18 + c.x + 1];
	}


	// Set the board to INITIAL BOARD
	void reset_board() {
		board.assign(INITIAL_BOARD);
	}


	// Place the Snake on the board
	void place_snake() {
		for (const Coord c : snake) {
			grab_char(c) = BODY;
		}
		grab_char(snake.front()) = HEAD;
	}


	// Place the food at the given location
	void place_food(const Coord loc) {
		if (loc.x > 14 || loc.y > 14) throw std::invalid_argument("Both x and y must be less than or equal to 14");

		if (grab_char(loc) != ' ') throw std::domain_error("The given space must be empty");

		grab_char(loc) = FOOD;
	}


	// Updates the snake's position given a specific input
	// Returns true when the snake successfully ate food
	bool update_snake(const Direction dir) {
		Coord next(snake.front());
		switch (dir) {
		case Direction::Up:
			++next.y;
			if (next.y > 14) alive = false;
			break;
		case Direction::Down:
			if (next.y == 0) {
				alive = false;
				break;
			}
			--next.y;
			break;
		case Direction::Left:
			if (next.x == 0) {
				alive = false;
				break;
			}
			--next.x;
			break;
		case Direction::Right:
			++next.x;
			if (next.x > 14) alive = false;
			break;
		}

		if (!alive) return false;

		bool ate = grab_char(next) == FOOD;

		if (ate) {
			snake.push_front(next);
			return true;
		}

		for(std::size_t i = 0; i < snake.size() - 1; ++i)
			if (snake[i] == next) {
				alive = false;
				return false;
			}

		snake.pop_back();
		snake.push_front(next);
		return false;
	}


	// Randomly generate the next food location in a valid space
	Coord generate_next_food() {
		std::deque<Coord> available;
		for (size_t y = 0; y < 15; ++y) {
			for (size_t x = 0; x < 15; ++x) {
				if (grab_char(Coord(x, y)) == ' ')
					available.emplace_back(x, y);
			}
		}

		if (available.empty()) return Coord(50, 50);

		std::uniform_int_distribution<size_t> dist(0, available.size() - 1);

		food = available[dist(*rng)];

		return food;
	}


	// Get the input from the player through stdin
	Direction get_input() {
		while (true) {
			unsigned char temp = _getch();
			if (temp == KEY_ARROW_CHAR1) {
				switch (_getch()) {
				case KEY_ARROW_UP:
					return Direction::Up;
				case KEY_ARROW_DOWN:
					return Direction::Down;
				case KEY_ARROW_LEFT:
					return Direction::Left;
				case KEY_ARROW_RIGHT:
					return Direction::Right;
				default:
					std::cout << "\nPressed unknown key: '" << temp << "'\nPlease use arrow keys." << std::endl;
					break;
				}
			}
			else {
				std::cout << "\nPressed unknown key: '" << temp << "'\nPlease use arrow keys." << std::endl;
			}
		}
	}


	// Play the failure message
	void lost() {
		std::cout << "You lost. Better luck next time!\nYou reached a score of "
				  << snake.size() << std::endl;
	}


	// Play the winning message
	void won() {
		std::cout << "You freak of nature. How did you actually do this? You filled the whole box. Congrats!\n"
				  << "You reached a score of " << snake.size() << std::endl;
	}


	// Play the intro message
	void intro() {
		std::cout << "You can control the snake using the arrow keys.\n"
			  << "Here is a legend of important characters:\n"
			  << "\t" << FOOD << ": This is the food you must collect to grow\n"
			  << "\t" << HEAD << ": The head of the snake\n"
			  << "\t" << BODY << ": The body of the snake\n"
			  << "\t+, -, and |: These are all border characters\n";
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
		intro();
		bool ate = true;
		while (alive) {
			place_snake();

			if (ate) {
				place_food(generate_next_food());
				ate = false;
			}
			else {
				place_food(food);
			}

			print_board();

			ate = update_snake(get_input());


			if (ate && snake.size() == MAX_LENGTH) {
				won();
				return;
			}

			reset_board();
		}
		lost();
	}

};

#endif
