#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

typedef struct {
    bool cell_filled[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
    bool active_tetromino[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH]; 
} Playfield;


Playfield playfield_constructor (void);

#endif
