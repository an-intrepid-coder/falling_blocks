#ifndef TETROMINO_H
#define TETROMINO_H

#define NUM_BLOCKS 4
#define NUM_TYPES 7
#define NEXT_WIDTH 4
#define NEXT_HEIGHT 2

#include "coord.h"
#include "playfield.h"

// The seven tetromino types represented as enums from 0-6.
typedef enum {SQUARE, STRAIGHT, SKEW_A, SKEW_B, L_A, L_B, T} Tetromino_Types;
// Tetromino configurations:
typedef enum {FIRST, SECOND, THIRD, FOURTH} Tetromino_Configurations;

typedef struct {
    /* An object representing a tetromino, and the (y, x) coordinates of its 4 blocks. 
     * Currently there is only *one* of these existing, and it does more than it should.
     * In the near future I'm going to re-do the tetromino system to use allocated "sprite"-
     * like objects and separate out the things that should be separated out, such as
     * the fact that the tetromino object also contains the "game_over" variable. The idea is
     * to eventually make a more generic system I can use for other games instead of something that
     * I hard-coded from the start to just play this one game.  */
    int tetromino_type;
    int tetromino_configuration;
    bool game_over;
    Coord blocks_yx[NUM_BLOCKS];
} Tetromino;

typedef struct {
    int length;
    int *types;
    int next_buffer[NEXT_HEIGHT][NEXT_WIDTH];
} Permutations_List;

Permutations_List new_permutations_list ();
int next_tetromino (Permutations_List *plist);
void plist_set_buffer(int tetromino_type, Permutations_List *plist);

Tetromino tetromino_constructor (int tetromino_type, int topleft_x, Playfield *playfield);
bool tetromino_can_move (Tetromino *tetromino, Playfield *playfield, int mvdir);
void tetromino_move (Tetromino *tetromino, Playfield *playfield, int mvdir);
void tetromino_freeze (Tetromino *tetromino, Playfield *playfield);
void tetromino_rotate (Tetromino *tetromino, Playfield *playfield);
bool tetromino_can_rotate (Tetromino *tetromino, Playfield *playfield);

#endif

