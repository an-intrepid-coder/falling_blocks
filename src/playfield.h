#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

typedef struct {
    /* The playfield object is just a collection of 2-D arrays representing
     * the "cells" that are the fundamental units of the game.  */
    bool cell_filled[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
    bool active_tetromino[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH]; 
} Playfield;


Playfield playfield_constructor (void);
bool playfield_row_filled (Playfield *playfield, int row);
void playfield_clear_row (Playfield *playfield, int row);
void playfield_gravity (Playfield *playfield, int row);
void playfield_clear_lines (Playfield *playfield);

#endif
