#include "tetromino.h"

Tetromino tetromino_constructor (int tetromino_type, int topleft_x) {
    /* constructor for the tetromino object. Takes a type from 0-6 and an x-coordinate
     * between 0-8.  */
    Tetromino new_tetromino;
    new_tetromino.tetromino_type = tetromino_type;
    new_tetromino.blocks_yx[0].y = 0; // will be -8 once drop zone and time steps implemented
    new_tetromino.blocks_yx[0].x = topleft_x;

    switch (tetromino_type) {
        /* The 3 remaining blocks are spawned relative to the top-left, according to
         * a fixed scheme.  */
        case SQUARE:
            new_tetromino.blocks_yx[1].y = 0;
            new_tetromino.blocks_yx[1].x = topleft_x + 1;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 1;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        case STRAIGHT:
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 2;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 3;
            new_tetromino.blocks_yx[3].x = topleft_x;
        break;
        case SKEW_A:
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x + 1;
            new_tetromino.blocks_yx[3].y = 2;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        case SKEW_B:
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 1;
            new_tetromino.blocks_yx[2].x = topleft_x - 1;
            new_tetromino.blocks_yx[3].y = 2;
            new_tetromino.blocks_yx[3].x = topleft_x - 1;
        break;
        case L_A:
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 2;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 2;
            new_tetromino.blocks_yx[3].x = topleft_x + 1;
        break;
        case L_B:
            new_tetromino.blocks_yx[1].y = 1;
            new_tetromino.blocks_yx[1].x = topleft_x;
            new_tetromino.blocks_yx[2].y = 2;
            new_tetromino.blocks_yx[2].x = topleft_x;
            new_tetromino.blocks_yx[3].y = 2;
            new_tetromino.blocks_yx[3].x = topleft_x - 1;
        break;
        case T:
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

    return new_tetromino;
}

// TO-DO: Rotation
