#pragma once

#ifndef VALUES_HPP
#define VALUES_HPP

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

#endif