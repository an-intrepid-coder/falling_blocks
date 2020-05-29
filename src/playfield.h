#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

#include "stats.h"

typedef enum {FILL_EMPTY, FILL_SQUARE, FILL_STRAIGHT, FILL_SKEW_A, FILL_SKEW_B, 
              FILL_L_A, FILL_L_B, FILL_T} Fill_Types;

typedef struct {
    /* The playfield object is just a collection of 2-D arrays representing
     * the "cells" that are the fundamental units of the game.  */
    bool active_tetromino[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH]; 
    int fill_type[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
} Playfield;


Playfield playfield_constructor (void);
bool playfield_row_filled (Playfield *playfield, int row);
void playfield_clear_row (Playfield *playfield, int row);
void playfield_gravity (Playfield *playfield, int row);
void playfield_clear_lines (Playfield *playfield, Stats *stats);

#endif
