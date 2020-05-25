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

bool playfield_row_filled (Playfield *playfield, int row) {
    /* Returns true if a row is filled.  */
    bool return_value = true;
    for (int cell = 0; cell < PLAYFIELD_WIDTH; cell++) {
        if (!playfield->cell_filled[row][cell]) {
            return_value = false;
            break;
        }
    }
    return return_value;
}

void playfield_clear_row (Playfield *playfield, int row) {
    /* Clears a line of blocks.  */
    for (int cell = 0; cell < PLAYFIELD_WIDTH; cell++) {
        playfield->cell_filled[row][cell] = false;
    }
}

void playfield_gravity (Playfield *playfield, int cleared_row) {
    /* Causes all cells to fall by 1 row.  */
    for (int row = cleared_row; row > 0; row--) {
        for (int cell = 0; cell < PLAYFIELD_WIDTH; cell++) {
            if (playfield->cell_filled[row - 1][cell]) {
                playfield->cell_filled[row - 1][cell] = false;
                playfield->cell_filled[row][cell] = true;
            }
        }
    }
}

void playfield_clear_lines (Playfield *playfield) {
    /* Loops through each row of cells on the playfield and
     * clears any filled lines.  */
    for (int row = PLAYFIELD_HEIGHT - 1; row > 0; row--) {
        if (playfield_row_filled(playfield, row)) { 
            playfield_clear_row(playfield, row);
            playfield_gravity(playfield, row);
            row = PLAYFIELD_HEIGHT;
        }
    }
}
