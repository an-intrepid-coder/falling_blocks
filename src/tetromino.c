#include "tetromino.h"

Permutations_List permutations_list_constructor () {
    /* A dynamic array that holds the available permutations.  */
    Permutations_List new_list;

    new_list.length = NUM_TYPES;
    new_list.types = malloc(NUM_TYPES * sizeof(int));
    if (new_list.types == NULL) {
        exit(1);
    }

    for (int type = 0; type < NUM_TYPES; type++) {
        new_list.types[type] = type;
    }

    return new_list;
}

int next_tetromino (Permutations_List *plist) {
    /* Returns an available tetromino type from a permutations list,
     * and then re-allocates it without the chosen element. Re-generates
     * a brand new list of tetromino types if the final element was chosen.  */
    int *new_types, choice = plist->types[rand() % plist->length];

    plist->length -= 1;
    if (plist->length == 0) {
        free(plist->types);
        *plist = permutations_list_constructor();
    } else {
        new_types = malloc(plist->length * sizeof(int));
        for (int index = 0, count = 0; index < plist->length + 1; index++) {
            if (plist->types[index] != choice) {
                new_types[count] = plist->types[index];
                count++;
            }
        }
        free(plist->types);
        plist->types = new_types;
    }
 
    return choice;
}

Tetromino tetromino_constructor (int tetromino_type, int topleft_x,
                                 Playfield *playfield) {
    /* constructor for the tetromino object. Takes a type from 0-6 and an x-coordinate
     * between 0-8.  */
    Tetromino new_tetromino = {
        .tetromino_type = tetromino_type,
        .tetromino_configuration = FIRST,
        .game_over = false,
    };

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
        int row = new_tetromino.blocks_yx[block].y;
        int cell = new_tetromino.blocks_yx[block].x;
        if (playfield->fill_type[row][cell] != FILL_EMPTY) {
            new_tetromino.game_over = true;
        } else {
            playfield->active_tetromino[row][cell] = true;
            playfield->fill_type[row][cell] = tetromino_type + 1;
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
                } else if (playfield->fill_type[tetromino->blocks_yx[block].y + 1]
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
                } else if (playfield->fill_type[tetromino->blocks_yx[block].y]
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
                } else if (playfield->fill_type[tetromino->blocks_yx[block].y]
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
                playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                           [tetromino->blocks_yx[block].x] = false;
                playfield->fill_type[tetromino->blocks_yx[block].y]
                                    [tetromino->blocks_yx[block].x] = FILL_EMPTY;
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
                playfield->active_tetromino[tetromino->blocks_yx[block].y]
                                           [tetromino->blocks_yx[block].x] = true;
                playfield->fill_type[tetromino->blocks_yx[block].y]
                                    [tetromino->blocks_yx[block].x] = 
                                        tetromino->tetromino_type + 1;
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


bool tetromino_can_rotate (Tetromino *tetromino, Playfield *playfield) {
    /* Returns true or false depending on whether or not the tetromino is able to rotate. This is
     * a naive implementation that needs to be refactored very badly. This was just to prove the 
     * concept.  */
    bool return_value = true;
    switch (tetromino->tetromino_type) {
        case SQUARE:
        break;
        case STRAIGHT:
            switch (tetromino->tetromino_configuration) {
                case FIRST:
                    for (int block = 0; block < 3; block++) {
                        if (tetromino->blocks_yx[1].y - block >= 0) {
                            if (playfield->fill_type[tetromino->blocks_yx[1].y - block]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y]
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
                        if (playfield->fill_type[tetromino->blocks_yx[0].y]
                                                [tetromino->blocks_yx[0].x - 1] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[0].y]
                                                        [tetromino->blocks_yx[0].x - 1]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[0].y - 1]
                                                 [tetromino->blocks_yx[0].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                         [tetromino->blocks_yx[0].x - 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[0].y]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[2].y]
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
                        if (playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                [tetromino->blocks_yx[0].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                        [tetromino->blocks_yx[0].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                 [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[1].y - 1]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[2].y + 1]
                                                 [tetromino->blocks_yx[2].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[2].y + 1]
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
                    if ((playfield->fill_type[tetromino->blocks_yx[0].y - 1]
                                             [tetromino->blocks_yx[0].x] &&
                         !playfield->active_tetromino[tetromino->blocks_yx[0].y - 1]
                                                     [tetromino->blocks_yx[0].x]) ||
                        (playfield->fill_type[tetromino->blocks_yx[0].y - 1]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[2].y]
                                                 [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[2].y]
                                                 [tetromino->blocks_yx[2].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 2]) ||
                            (playfield->fill_type[tetromino->blocks_yx[2].y + 1]
                                                 [tetromino->blocks_yx[2].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y + 1]
                                                         [tetromino->blocks_yx[2].x + 2])) {
                            return_value = false;
                        }
                    }
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if ((playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                 [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                 [tetromino->blocks_yx[0].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x + 1])) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                 [tetromino->blocks_yx[0].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[0].y + 1]
                                                 [tetromino->blocks_yx[0].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[0].y + 1]
                                                         [tetromino->blocks_yx[0].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[0].y - 1]
                                                 [tetromino->blocks_yx[0].x + 1])) {
                            return_value = false;
                        }
                    }
                break;
                case FOURTH:
                    if (tetromino->blocks_yx[0].x + 1 > (PLAYFIELD_WIDTH - 1)) {
                        return_value = false;
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[2].y - 1]
                                                 [tetromino->blocks_yx[2].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y - 1]
                                                         [tetromino->blocks_yx[2].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y]
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
                        if (playfield->fill_type[tetromino->blocks_yx[1].y - 1]
                                                [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[1].y - 1]
                                                 [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[1].y - 2]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[1].y]
                                                 [tetromino->blocks_yx[1].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y]
                                                         [tetromino->blocks_yx[1].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[1].y]
                                                 [tetromino->blocks_yx[1].x + 2] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y]
                                                         [tetromino->blocks_yx[1].x + 2])) {
                            return_value = false;
                        }
                    } 
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->fill_type[tetromino->blocks_yx[1].y + 1]
                                                [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y + 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[1].y + 1]
                                                 [tetromino->blocks_yx[1].x] &&
                             !playfield->fill_type[tetromino->blocks_yx[1].y + 1]
                                                  [tetromino->blocks_yx[1].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[1].y - 1]
                                                 [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[0].y - 1]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y - 1]
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
                        if (playfield->fill_type[tetromino->blocks_yx[1].y - 1]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x - 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x - 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[3].y]
                                                 [tetromino->blocks_yx[3].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[3].y]
                                                         [tetromino->blocks_yx[3].x + 1])) {
                            return_value = false;
                        }
                    } 
                break;
                case THIRD:
                    if (tetromino->blocks_yx[0].y - 1 < 0) {
                        if (playfield->fill_type[tetromino->blocks_yx[1].y - 1]
                                                [tetromino->blocks_yx[1].x] &&
                            !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                        [tetromino->blocks_yx[1].x]) {
                            return_value = false;
                        }
                    } else {
                        if ((playfield->fill_type[tetromino->blocks_yx[1].y - 1]
                                                 [tetromino->blocks_yx[1].x] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[1].y - 1]
                                                         [tetromino->blocks_yx[1].x]) ||
                            (playfield->fill_type[tetromino->blocks_yx[1].y - 2]
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
                        if ((playfield->fill_type[tetromino->blocks_yx[2].y]
                                                 [tetromino->blocks_yx[2].x + 1] &&
                             !playfield->active_tetromino[tetromino->blocks_yx[2].y]
                                                         [tetromino->blocks_yx[2].x + 1]) ||
                            (playfield->fill_type[tetromino->blocks_yx[2].y + 1]
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
     * blocks whose coordinates don't change during rotation. Basically everything about
     * the rotation scheme was done ad-hoc and I plan on doing it again more deliberately.  */
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

