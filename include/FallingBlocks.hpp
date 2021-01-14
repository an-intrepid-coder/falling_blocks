#ifndef FALLINGBLOCKS_HPP
#define FALLINGBLOCKS_HPP

#define FRAME_WAIT 33ms
#define DEFAULT_GRAVITY 1000
#define GAME_HEIGHT 24
#define GAME_WIDTH 30
#define LINE_SCORE 100
#define BONUS_SCORE 1000
#define LINES_TO_LEVEL 20
#define MS_DECREMENT 75
#define MS_FLOOR 45

#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>
#include "Playfield.hpp"
#include "Tetromino.hpp"
#include "TetrominoGenerator.hpp"

class FallingBlocks
{
    public:
        FallingBlocks(int starting_level, bool solid_color);
        ~FallingBlocks();

        unsigned long int game_loop();

    private:
        void init_curses(bool solid_color);
        void uninit_curses();
        void draw_game();
        void level_up();
        void pause();
        int convert_input(int input);

        int level;
        unsigned long int lines_cleared, score, lines_this_level;
        Playfield playfield = Playfield(PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH);
        TetrominoGenerator generator = TetrominoGenerator();
        Tetromino tetromino = generator.next(playfield);
        std::chrono::milliseconds gravity_threshold{DEFAULT_GRAVITY};
        std::chrono::time_point<std::chrono::high_resolution_clock> gravity_clock = std::chrono::high_resolution_clock::now();
};

#endif // FALLINGBLOCKS_HPP
