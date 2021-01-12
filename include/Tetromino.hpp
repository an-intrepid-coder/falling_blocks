#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <vector>
#include "Coord.hpp"
#include "Block.hpp"
#include "Playfield.hpp"

#define NUM_TETROMINOS 7

enum { TETROMINO_NONE, TETROMINO_SQUARE, TETROMINO_STRAIGHT, TETROMINO_SKEW_A, TETROMINO_SKEW_B, TETROMINO_L_A, TETROMINO_L_B, TETROMINO_T }; // TetrominoTypes;

enum { MOVE_NONE, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_ROTATE, MOVE_PAUSE }; // MoveDirs;

class Tetromino
{
    public:
        Tetromino(Coord origin, int type);

        void attempt_move(Playfield& playfield, int move_type);
        bool resting(Playfield& playfield);
        void freeze(Playfield& playfield);
        void clockwise_rotation(Playfield& playfield);
        void draw(Coord origin, Playfield& playfield);

    private:
        bool valid_placement(Playfield& playfield, Block& old_block, Coord& target);
        std::vector<std::vector<Block>> fresh_blocks();
        void offset(int& dy, int& dx);

        int type, orientation, field_size;
        std::vector<std::vector<Block>> blocks;
};

#endif // TETROMINO_HPP
