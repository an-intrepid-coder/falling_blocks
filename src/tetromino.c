#include "tetromino.h"

Tetromino tetromino_constructor (int tetromino_type, int topleft_x,
                                 bool playfield[][PLAYFIELD_WIDTH]) {
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
        playfield[y][x] = true;
    }

    return new_tetromino;
}

// TO-DO: Rotation
