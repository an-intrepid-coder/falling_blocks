#ifndef TETROMINO_H
#define TETROMINO_H

#define NUM_BLOCKS 4

#include "coord.h"

// The seven tetromino types represented as enums from 0-6.
typedef enum {SQUARE, STRAIGHT, SKEW_A, SKEW_B, L_A, L_B, T} Tetromino_Types;

typedef struct {
    /* An object representing a tetromino, and the (y, x) coordinates of its 4 blocks.  */
    int tetromino_type;
    Coord blocks_yx[NUM_BLOCKS];
} Tetromino;

Tetromino tetromino_constructor (int tetromino_type, int topleft_x);
bool tetromino_drop (Tetromino *tetromino);
bool tetromino_can_fall(Tetromino *tetromino);

#endif
