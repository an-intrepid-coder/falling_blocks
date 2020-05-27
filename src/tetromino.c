#include "tetromino.h"

Tetromino tetromino_constructor (int tetromino_type, int topleft_x,
                                 Playfield *playfield) {
    /* constructor for the tetromino object. Takes a type from 0-6 and an x-coordinate
     * between 0-8.  */
    Tetromino new_tetromino;
    new_tetromino.tetromino_type = tetromino_type;
    new_tetromino.tetromino_configuration = FIRST;
    new_tetromino.game_over = false;

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
    }

    for (int block = 0; block < NUM_BLOCKS; block++) {
        int y = new_tetromino.blocks_yx[block].y;
        int x = new_tetromino.blocks_yx[block].x;
        if (playfield->cell_filled[y][x]) {
            new_tetromino.game_over = true;
        } else {
            playfield->cell_filled[y][x] = true;
            playfield->active_tetromino[y][x] = true;
        }
    }

    return new_tetromino;
}

bool tetromino_can_move(Tetromino *tetromino, Playfield *playfield, int mvdir) {
    /* Returns true if the tetromino can move in the indicated direction.  */
    bool return_value = true;
    switch (mvdir) {
        case DOWN:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].y >= (PLAYFIELD_HEIGHT - 1)) {
                    return_value = false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y + 1]
                                                 [tetromino->blocks_yx[block].x] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y + 1] 
                                                       [tetromino->blocks_yx[block].x]) {
                    return_value = false;
                }
            }
        break;
        case LEFT:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].x <= 0) {
                    return_value = false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y]
                                                 [tetromino->blocks_yx[block].x - 1] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y] 
                                                       [tetromino->blocks_yx[block].x - 1]) {
                    return_value = false;
                }
            }
        break;
        case RIGHT:
            for (int block = 0; block < NUM_BLOCKS; block++) {
                if (tetromino->blocks_yx[block].x >= (PLAYFIELD_WIDTH - 1)) {
                    return_value = false;
                } else if (playfield->cell_filled[tetromino->blocks_yx[block].y]
                                                 [tetromino->blocks_yx[block].x + 1] &&
                           !playfield->active_tetromino[tetromino->blocks_yx[block].y] 
                                                       [tetromino->blocks_yx[block].x + 1]) {
                    return_value = false;
                }
            }
        break;
        case DIRECTION_NONE:
            return_value = false;
        default:
        break;
    }
    return return_value;
}

void tetromino_move(Tetromino *tetromino, Playfield *playfield, int mvdir) {
    if (tetromino_can_move(tetromino, playfield, mvdir)) {
        /* REFACTOR NOTE: This for-loop and its companion at the end of the function are prime 
         *                candidates for later refactoring.  */
        for (int block = 0; block < NUM_BLOCKS; block++) {
            if (tetromino->blocks_yx[block].y >= 0) {
                playfield->cell_filled[tetromino->blocks_yx[block].y]
                                      [tetromino->blocks_yx[block].x] = false;
                playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                           [tetromino->blocks_yx[block].x] = false;
            }
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
            case ROTATE:
                tetromino_rotate(tetromino, playfield);
            break;
            default:
            break;
        }
        for (int block = 0; block < NUM_BLOCKS; block++) {
            if (tetromino->blocks_yx[block].y >= 0) {
                playfield->cell_filled[tetromino->blocks_yx[block].y]
                                      [tetromino->blocks_yx[block].x] = true;
                playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                           [tetromino->blocks_yx[block].x] = true;
            }
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

/* REFACTOR NOTE: This function will be broken down into three or four
 *                sub-functions after it is in an operational state.  */
bool tetromino_can_rotate (Tetromino *tetromino, Playfield *playfield) {
    /* Returns true or false depending on whether or not the tetromino is able to rotate.  */
    bool return_value = true;
    switch (tetromino->tetromino_type) {
        case SQUARE:
        break;
        case STRAIGHT:
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    for (int block = 0; block < 3; block++) {
                        if (tetromino->blocks_yx[1].y - block >= 0) {
                            if (playfield->cell_filled[tetromino->blocks_yx[1].y - block]
                                                      [tetromino->blocks_yx[1].x] &&
                                !playfield->active_tetromino[tetromino->blocks_yx[1].y - block]
                                                            [tetromino->blocks_yx[1].x]) {
                                return_value = false;
                            }
                        }
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[3].x - 1 >= 0 &&
                        tetromino->blocks_yx[3].x + 1 <= (PLAYFIELD_WIDTH - 1) &&
                        tetromino->blocks_yx[3].x + 2 <= (PLAYFIELD_WIDTH - 1)) {
                        if ((playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 2])) {
                            return_value = false;
                        }
                    } else {    
                        return_value = false;
                    }
                break;
            }
        break;
        case SKEW_A:
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[0].y]
                                                  [tetromino->blocks_yx[0].x - 1] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[0].y]
                                                        [tetromino->blocks_yx[0].x - 1]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[0].y - 1]
                                                   [tetromino->blocks_yx[0].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                         [tetromino->blocks_yx[0].x - 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[0].y]
                                                   [tetromino->blocks_yx[0].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y]
                                                         [tetromino->blocks_yx[0].x - 1])) {
                            return_value = false;
                        }
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[3].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[2].y]
                                                   [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
            }
        break;
        case SKEW_B:
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                  [tetromino->blocks_yx[0].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                        [tetromino->blocks_yx[0].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                   [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x])) {
                            return_value = false;
                        }
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[0].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[2].y + 1]
                                                   [tetromino->blocks_yx[2].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[2].y + 1]
                                                   [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
            }
        break;
        case L_B:
            // clockwise only for now
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    if ((playfield->cell_filled[tetromino->blocks_yx[0].y - 1]
                                               [tetromino->blocks_yx[0].x] &&
                         !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                     [tetromino->blocks_yx[0].x]) ||
                        (playfield->cell_filled[tetromino->blocks_yx[0].y - 1]
                                               [tetromino->blocks_yx[0].x + 1] &&
                         !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                     [tetromino->blocks_yx[0].x + 1])) {
                        return_value = false;
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[1].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[2].y]
                                                   [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[2].y]
                                                   [tetromino->blocks_yx[2].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 2]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[2].y + 1]
                                                   [tetromino->blocks_yx[2].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x + 2])) {
                            return_value = false;
                        }
                    }
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if ((playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                   [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                   [tetromino->blocks_yx[0].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x + 1])) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                   [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[0].y + 1]
                                                   [tetromino->blocks_yx[0].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[0].y - 1]
                                                   [tetromino->blocks_yx[0].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
                case FOURTH:
                    if (tetromino->blocks_yx[0].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[2].y - 1]
                                                   [tetromino->blocks_yx[2].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y - 1]
                                                         [tetromino->blocks_yx[2].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
            }
        break;
        case L_A:
            // clockwise only for now
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                  [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[1].y - 2]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 2]
                                                         [tetromino->blocks_yx[1].x])) {
                            return_value = false;
                        }
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[3].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[1].y]
                                                   [tetromino->blocks_yx[1].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y]
                                                         [tetromino->blocks_yx[1].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[1].y]
                                                   [tetromino->blocks_yx[1].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y]
                                                         [tetromino->blocks_yx[1].x + 2])) {
                            return_value = false;
                        }
                    } 
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[1].y + 1]
                                                  [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y + 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[1].y + 1]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->cell_filled[tetromino->blocks_yx[1].y + 1]
                                                    [tetromino->blocks_yx[1].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[0].y - 1]
                                                   [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                         [tetromino->blocks_yx[0].x])) {
                            return_value = false;
                        }
                    }
                break;
                case FOURTH:
                    if (tetromino->blocks_yx[1].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y - 1]
                                                   [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y - 1]
                                                         [tetromino->blocks_yx[3].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
            }
        break;
        case T:
            // clockwise only for now
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    if (tetromino->blocks_yx[0].y - 1 > 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                  [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    }
                break;
                case SECOND:
                    if (tetromino->blocks_yx[0].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[3].y]
                                                   [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1])) {
                            return_value = false;
                        }
                    } 
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                  [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[1].y - 1]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[1].y - 2]
                                                   [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 2]
                                                         [tetromino->blocks_yx[1].x])) {
                            return_value = false;
                        }
                    }
                break;
                case FOURTH:
                    if (tetromino->blocks_yx[2].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->cell_filled[tetromino->blocks_yx[2].y]
                                                   [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 1]) ||
                            (playfield->cell_filled[tetromino->blocks_yx[2].y + 1]
                                                   [tetromino->blocks_yx[2].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x])) {
                            return_value = false;
                        }
                    }
                break;
            }
        break;
    }
    return return_value;
}

void tetromino_rotate (Tetromino *tetromino, Playfield *playfield) {
    /* Will destroy/rebuild tetromino objects to give the appearance of clockwise rotation.
     * State-changes will be relative to a block that does not move, but sometimes the
     * non-moving block will "switch indexes" with another block because the indexing scheme
     * is pretty arbitrary and should be refactored to specifically take advantage of 
     * blocks whose coordinates don't change during rotation.  */
    if (tetromino_can_rotate(tetromino, playfield)) {
        switch (tetromino->tetromino_type) {
            case STRAIGHT:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 3;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y - 2;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[3].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[3].x + 1;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[3].x + 2;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
            case SQUARE:
                // Square shape does not require anything here.
            break;
            case SKEW_A:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[3].y - 2;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[3].x;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[3].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[3].x + 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
            case SKEW_B:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x - 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x - 1;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[2].x - 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[2].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[2].x + 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[2].x;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
            case L_A:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 2;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x + 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x + 2;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x + 1;
                        tetromino->tetromino_configuration = THIRD;
                    break;
                    case THIRD:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x - 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x;
                        tetromino->tetromino_configuration = FOURTH;
                    break;
                    case FOURTH:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[3].x + 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[3].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[3].x + 1;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
            case L_B:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[0].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[0].x + 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[0].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[0].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[0].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[0].x;
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[0].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[0].x;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[2].x + 1;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[2].x + 2;
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[2].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[2].x + 2;
                        tetromino->tetromino_configuration = THIRD;
                    break;
                    case THIRD:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x - 1;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->tetromino_configuration = FOURTH;
                    break;
                    case FOURTH:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[3].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[3].x - 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[3].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[3].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[3].x + 1;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
            case T:
                switch (tetromino->tetromino_configuration) {
                    case FIRST:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x - 1;
                        tetromino->tetromino_configuration = SECOND;
                    break;
                    case SECOND:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[2].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[2].x;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[2].x + 1;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[2].x - 1;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[2].y + 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[2].x;
                        tetromino->tetromino_configuration = THIRD;
                    break;
                    case THIRD:
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y - 2;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x + 1;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y - 1;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x;
                        tetromino->tetromino_configuration = FOURTH;
                    break;
                    case FOURTH:
                        tetromino->blocks_yx[2].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[2].x = tetromino->blocks_yx[1].x + 2;
                        tetromino->blocks_yx[3].y = tetromino->blocks_yx[1].y + 1;
                        tetromino->blocks_yx[3].x = tetromino->blocks_yx[1].x + 1;
                        tetromino->blocks_yx[0].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[0].x = tetromino->blocks_yx[1].x;
                        tetromino->blocks_yx[1].y = tetromino->blocks_yx[1].y;
                        tetromino->blocks_yx[1].x = tetromino->blocks_yx[1].x + 1;
                        tetromino->tetromino_configuration = FIRST;
                    break;
                }
            break;
        }
    }
}
