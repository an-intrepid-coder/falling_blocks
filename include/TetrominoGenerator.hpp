#ifndef TETROMINOGENERATOR_HPP
#define TETROMINOGENERATOR_HPP

#define GENERATOR_SHUFFLES 100
#define NEW_ROTATIONS_LIMIT 10
#define NEW_SHIFTS_LIMIT 8
#define STARTING_ROW -4

#include <deque>
#include <random>
#include "Tetromino.hpp"
#include "Playfield.hpp"

class TetrominoGenerator
{
    public:
        TetrominoGenerator();

        int preview() { return permutations.front(); }
        Tetromino next(Playfield& playfield);

    private:
        void new_batch();

        std::linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647> generator;
        std::deque<int> permutations;
};

#endif // TETROMINOGENERATOR_HPP
