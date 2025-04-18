#pragma once

#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Audio.hpp"
#include <conio.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <time.h>

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


	// Useful constants
	static constexpr char INITIAL_BOARD[] = "+---------------+\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n|               |\n+---------------+";
	static constexpr char HEAD = 'H';
	static constexpr char BODY = 'X';
	static constexpr char FOOD = 'O';
	static constexpr unsigned char KEY_ARROW_CHAR1 = 224;
	static constexpr unsigned char KEY_ARROW_UP = 80;
	static constexpr unsigned char KEY_ARROW_DOWN = 72;
	static constexpr unsigned char KEY_ARROW_LEFT = 75;
	static constexpr unsigned char KEY_ARROW_RIGHT = 77;
	static constexpr unsigned char KEY_ESCAPE = 27;
	static constexpr char BG_MUSIC[] = "music.wav";
	static constexpr char DEATH_SOUND[] = "death.wav";
	static constexpr char FOOD_SOUND[] = "food.wav";
	static constexpr char VICTORY_MUSIC[] = "victory.wav";


	// A 15 x 15 board for the game plus a border making the string 17 x 17
	std::string board;

	// Holds the locations of the snake (index 0 is always head)
	std::deque<Coord> snake;

	// Holds the current location of the food
	Coord food;

	// The rng for determining the next food location
	std::unique_ptr<std::mt19937> rng;

	// The maximum length of the snake. When reached, you win.
	constexpr static size_t MAX_LENGTH = 225;

	// The previous high score of all players on this machine (-1 if no high score is set)
	int highscore = -1;

	// Whether to write to highscore.txt on game finish (write on true)
	bool write = true;

	// True while the snake is still alive
	bool alive = true;

	// The audio engine for the game
	Audio audio;

	// Volume control that the player can choose [0 - 1]
	float volume = 1;


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
		if (write) output_hs();
	}


	// Play the winning message
	void won() {
		std::cout << "You freak of nature. How did you actually do this? You filled the whole box. Congrats!\n"
				  << "You reached a score of " << snake.size() << std::endl;
		if (write) output_hs();
	}


	// Output highscore results to player
	void output_hs() {
		if (highscore == -1) {
			std::cout << "Congratulations, you set the first highcore of " << snake.size() << '.' << std::endl;
			write_hs();
		}
		else if (snake.size() > highscore) {
			std::cout << "Congratulations, you have beat the previous of highscore of "
					  << highscore << " by " << snake.size() - highscore
					  << "\nThe new highscore is " << snake.size() << '.' << std::endl;
			write_hs();
		}
		else {
			std::cout << "The previous highscore still stands. You are still " << highscore - snake.size()
					  << " from the current highscore of " << highscore << '.' << std::endl;
		}
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


	// Ask the player if it's okay to overwrite a poorly formatted highscore.txt file
	void ask_write_hs() {
		std::cout << "`highscore.txt` was incorrectly formatted and cannot be correctly read.\n"
				  << "Would you like to overwrite the old `highscore.txt`? (Y/n)\n";

		std::string buffer;
		std::cin >> buffer;

		while (buffer != "y" && buffer != "Y" && buffer != "n" && buffer != "N")
			std::cin >> buffer;
		std::cout << "This is buffer after loop: " << buffer << std::endl;
		if (buffer == "n" || buffer == "N") {
			std::cout << "Set write to false" << std::endl;
			write = false;
		}

		highscore = -1;
	}


	// Grab the high score from highscore.txt
	void read_highscore() {
		std::ifstream fin("highscore.txt");

		if (!fin.is_open()) {
			highscore = -1;
			return;
		}

		std::string buffer;
		fin >> buffer;

		if (buffer != "highscore:") {
			ask_write_hs();
			return;
		}

		fin >> buffer;

		highscore = std::stoi(buffer);
	}


	// Write the player's score to highscore.txt
	void write_hs() {
		std::ofstream fout("highscore.txt");

		if (!fout.is_open()) return;

		fout << "highscore: " << snake.size();
	}

public:

	// Basic constructor for setting up the initial conditions of the board
	Snake() : board{ INITIAL_BOARD }, rng{ new std::mt19937(time(NULL)) }, audio{ "" } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Provide a specfic seed for the rng
	Snake(unsigned int seed) : board{ INITIAL_BOARD }, rng{ new std::mt19937(seed) }, audio{ "" } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Provide a path for audio with char*
	Snake(char* path) : board{ INITIAL_BOARD }, rng{ new std::mt19937(time(NULL)) }, audio{ path } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Provide a path for audio with std::string
	Snake(std::string path) : board{ INITIAL_BOARD }, rng{ new std::mt19937(time(NULL)) }, audio{ path } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Provide a path for audio with char* and a seed for the rng
	Snake(char* path, unsigned int seed) : board{ INITIAL_BOARD }, rng{ new std::mt19937(seed) }, audio{ path } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Provide a path for audio with std::string and a seed for the rng
	Snake(std::string path, unsigned int seed) : board{ INITIAL_BOARD }, rng{ new std::mt19937(seed) }, audio{ path } {
		snake.emplace_back(7, 7);
		snake.emplace_back(6, 7);
		snake.emplace_back(5, 7);
		read_highscore();
	}


	// Starts a game of snake
	void play() {
		audio.Play(BG_MUSIC, 0.5, true);
		intro();
		bool ate = true;
		bool first = true;
		while (alive) {
			place_snake();

			if (ate) {
				if (!first) audio.Play(FOOD_SOUND, 1.0, false);
				place_food(generate_next_food());
				ate = false;
				first = false;
			}
			else {
				place_food(food);
			}

			print_board();

			ate = update_snake(get_input());


			if (ate && snake.size() == MAX_LENGTH) {
				audio.EndLoop(BG_MUSIC);
				audio.Play(VICTORY_MUSIC, 1.0, true);
				won();
				return;
			}

			reset_board();
		}

		audio.EndLoop(BG_MUSIC);
		audio.Play(DEATH_SOUND, 1.0, false);
		lost();
	}

};

#endif
