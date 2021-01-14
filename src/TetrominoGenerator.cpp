#include <chrono>
#include "TetrominoGenerator.hpp"

using std::deque;
using std::chrono::system_clock;
using std::linear_congruential_engine;

TetrominoGenerator::TetrominoGenerator()
{
    unsigned long int seed = system_clock::to_time_t(system_clock::now());
    generator = linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647>(seed);
    new_batch();
}

void TetrominoGenerator::new_batch()
{
    deque<int> new_permutations;
    for (auto index = 1; index <= NUM_TETROMINOS; index++)
        new_permutations.push_back(index);

    for (auto shuffle = 0; shuffle < GENERATOR_SHUFFLES; shuffle++)
    {
        int i = generator() % NUM_TETROMINOS, j = generator() % NUM_TETROMINOS;
        if (i != j)
        {
            int temp = new_permutations[i];
            new_permutations[i] = new_permutations[j];
            new_permutations[j] = temp;
        }
    }
    permutations = new_permutations;
}

Tetromino TetrominoGenerator::next(Playfield& playfield)
{
    Tetromino next = Tetromino(Coord(STARTING_ROW, 0), permutations.front());
    int rotations = generator() % NEW_ROTATIONS_LIMIT, shifts = generator() % NEW_SHIFTS_LIMIT;
    for (auto moves = 0; moves < shifts; moves++)
            next.attempt_move(playfield, MOVE_RIGHT);
    if (next.get_type() != TETROMINO_SQUARE)
    {
        for (auto moves = 0; moves < rotations; moves++)
            next.clockwise_rotation(playfield);
    }

    permutations.pop_front();
    if (permutations.empty())
        new_batch();

    return next;
}
