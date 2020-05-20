#include "tetromino.h"

Tetromino tetromino_constructor (int tetromino_type, int topleft_x,
                                 Playfield *playfield) {
    /* constructor for the tetromino object. Takes a type from 0-6 and an x-coordinate
     * between 0-8.  */
    Tetromino new_tetromino;
    new_tetromino.tetromino_type = tetromino_type;

    switch (tetromino_type) {
        /* The blocks are spawned according to a fixed scheme, relative to the "topleft" 
         * x-coordinate of the first block's "area". 
         * Example:
         *             top-left cell in area for L_A -> x.#
         *                                              ###  */ 
        case SQUARE:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x;
            new_tetromino.blocks_yx[1].y = 0;
            new_tetromino.blocks_yx[1].x = topleft_x + 1;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        case STRAIGHT:
            new_tetromino.blocks_yx[0].y = 1; 
            new_tetromino.blocks_yx[0].x = topleft_x;
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x + 1;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x + 2;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 3;
        break;
        case SKEW_A:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x + 1;
            new_tetromino.blocks_yx[1].y = 0;
            new_tetromino.blocks_yx[1].x = topleft_x + 2;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        case SKEW_B:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x;
            new_tetromino.blocks_yx[1].y = 0;
            new_tetromino.blocks_yx[1].x = topleft_x + 1;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x + 1;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 2;
        break;
        case L_A:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x + 2;
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x + 1;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 2;
        break;
        case L_B:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x;
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x + 1;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 2;
        break;
        case T:
            new_tetromino.blocks_yx[0].y = 0; 
            new_tetromino.blocks_yx[0].x = topleft_x;
            new_tetromino.blocks_yx[1].y = 0;
            new_tetromino.blocks_yx[1].x = topleft_x + 1;
            new_tetromino.blocks_yx[2].y = 0;
            new_tetromino.blocks_yx[2].x = topleft_x + 2;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        default:
            // to-do: Error handling
        break;
    }

    for (int block = 0; block < NUM_BLOCKS; block++) {
        int y = new_tetromino.blocks_yx[block].y;
        int x = new_tetromino.blocks_yx[block].x;
        playfield->cell_filled[y][x] = true;
        playfield->active_tetromino[y][x] = true;
    }

    return new_tetromino;
}

bool tetromino_can_move(Tetromino *tetromino, Playfield *playfield, int mvdir) {
    /* Needs to check if at bottom of screen OR if would collide  */
    switch (mvdir) {
        case DOWN:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].y >= (PLAYFIELD_HEIGHT - 1)) {
                    return false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y + 1]
                                                 [tetromino->blocks_yx[block].x] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y + 1] 
                                                       [tetromino->blocks_yx[block].x]) {
                    return false;
                }
            }
        break;
        case LEFT:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].x <= 0) {
                    return false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y]
                                                 [tetromino->blocks_yx[block].x - 1] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y] 
                                                       [tetromino->blocks_yx[block].x - 1]) {
                    return false;
                }
            }
        break;
        case RIGHT:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].x >= (PLAYFIELD_WIDTH - 1)) {
                    return false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y]
                                                 [tetromino->blocks_yx[block].x + 1] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y] 
                                                       [tetromino->blocks_yx[block].x + 1]) {
                    return false;
                }
            }
        break;
        case NONE:
            return false;
        default:
        break;
    }
    return true;
}

void tetromino_move(Tetromino *tetromino, Playfield *playfield, int mvdir) {
    if (tetromino_can_move(tetromino, playfield, mvdir)) {
        for (int block = 0; block < NUM_BLOCKS; block++) {
            playfield->cell_filled[tetromino->blocks_yx[block].y]
                                  [tetromino->blocks_yx[block].x] = false;
            playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                       [tetromino->blocks_yx[block].x] = false;
        }
        switch (mvdir) {
            case DOWN:
                for (int block = 0; block < NUM_BLOCKS; block++) {
                    tetromino->blocks_yx[block].y += 1;
                }
            break;
            case LEFT:
                for (int block = 0; block < NUM_BLOCKS; block++) {
                    tetromino->blocks_yx[block].x -= 1;
                }
            break;
            case RIGHT:
                for (int block = 0; block < NUM_BLOCKS; block++) {
                    tetromino->blocks_yx[block].x += 1;
                }
            break;
            default:
            break;
        }
        for (int block = 0; block < NUM_BLOCKS; block++) {
            playfield->cell_filled[tetromino->blocks_yx[block].y]
                                  [tetromino->blocks_yx[block].x] = true;
            playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                       [tetromino->blocks_yx[block].x] = true;
        }
    }
}

void tetromino_freeze (Tetromino *tetromino, Playfield *playfield) {
    /* "freezes" the tetromino in place by deactivating it on the playfield.  */
    for (int block = 0; block < NUM_BLOCKS; block++) {
        playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                   [tetromino->blocks_yx[block].x] = false;
    }
}

// TO-DO: Rotation
