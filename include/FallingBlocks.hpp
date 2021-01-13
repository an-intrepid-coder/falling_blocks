#ifndef FALLINGBLOCKS_HPP
#define FALLINGBLOCKS_HPP

#define DEFAULT_GRAVITY 1000

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
        FallingBlocks();
        ~FallingBlocks();

        void game_loop(); // can eventually return a score or something

    private:
        void init_curses ();
        void uninit_curses ();
        void draw_game();
        int convert_input(int input);

        int level;
        Playfield playfield = Playfield(24, 10);
        TetrominoGenerator generator = TetrominoGenerator();
        Tetromino tetromino = generator.next(playfield);
        std::chrono::milliseconds gravity_threshold{DEFAULT_GRAVITY};
        std::chrono::time_point<std::chrono::high_resolution_clock> gravity_clock = std::chrono::high_resolution_clock::now();
};

#endif // FALLINGBLOCKS_HPP
