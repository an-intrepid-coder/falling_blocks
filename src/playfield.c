#include "playfield.h"

Playfield playfield_constructor (void) {
    Playfield new_playfield;

    for (int row = 0; row < PLAYFIELD_HEIGHT; row++) {
        for (int col = 0; col < PLAYFIELD_WIDTH; col++) {
            new_playfield.cell_filled[row][col] = false;
            new_playfield.active_tetromino[row][col] = false;
        }
    }

    return new_playfield;
}
