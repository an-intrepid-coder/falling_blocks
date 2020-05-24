#ifndef TETROMINO_H
#define TETROMINO_H

#define NUM_BLOCKS 4

#include "coord.h"
#include "playfield.h"

// The seven tetromino types represented as enums from 0-6.
typedef enum {SQUARE, STRAIGHT, SKEW_A, SKEW_B, L_A, L_B, T} Tetromino_Types;
// Tetromino configurations:
typedef enum {FIRST, SECOND, THIRD, FOURTH} Tetromino_Configurations;

typedef struct {
    /* An object representing a tetromino, and the (y, x) coordinates of its 4 blocks.  */
    int tetromino_type;
    int tetromino_configuration;
    Coord blocks_yx[NUM_BLOCKS];
} Tetromino;

Tetromino tetromino_constructor (int tetromino_type, int topleft_x, Playfield *playfield);
bool tetromino_can_move (Tetromino *tetromino, Playfield *playfield, int mvdir);
void tetromino_move (Tetromino *tetromino, Playfield *playfield, int mvdir);
void tetromino_freeze (Tetromino *tetromino, Playfield *playfield);
void tetromino_rotate (Tetromino *tetromino, Playfield *playfield);
bool tetromino_can_rotate (Tetromino *tetromino, Playfield *playfield);

#endif
