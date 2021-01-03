#ifndef FALLING_BLOCKS_H
#define FALLING_BLOCKS_H

#define GAME_SCREEN_HEIGHT 24
#define GAME_SCREEN_WIDTH 24
#define HUD_HEIGHT 24
#define HUD_WIDTH 14

#define FRAME_INTERVAL (double) .033
#define NSECS_PER_SEC 1000000000

#define SPAWN_LIMIT 7

#include <time.h>
#include "coord.h"
#include "tetromino.h"
#include "playfield.h"
#include "stats.h"

// Enum for the hud display rows.
typedef enum {SCORE_LINE = 0, LEVEL_LINE = 2, NEXT_LINE = 4} Hud_Lines;

// Tetromino input enum 
typedef enum {DIRECTION_NONE, DOWN, LEFT, RIGHT, ROTATE, PAUSE} Input_Types;

// Color pairs
typedef enum { 
    PAIR_NORMAL,

    PAIR_SQUARE_ASCII, PAIR_STRAIGHT_ASCII, PAIR_SKEW_A_ASCII, PAIR_SKEW_B_ASCII,
    PAIR_L_A_ASCII, PAIR_L_B_ASCII, PAIR_T_ASCII,

    PAIR_SQUARE_SOLID, PAIR_STRAIGHT_SOLID, PAIR_SKEW_A_SOLID, PAIR_SKEW_B_SOLID,
    PAIR_L_A_SOLID, PAIR_L_B_SOLID, PAIR_T_SOLID
} Color_Pairs;

typedef struct {
    int pair;
    int symbol;
} Block_Attr;

// Color modes
typedef enum {MODE_NO_COLOR, MODE_ASCII_COLOR, MODE_SOLID_COLOR} Color_Modes;

bool game_fits_screen ();
void pause_game ();
Block_Attr get_block_attr(int color_mode, int fill_type);
void draw_game (Playfield *playfield, Stats *stats, Permutations_List *plist, 
                int color_mode);
void print_debug (Stats *stats, Tetromino *tetromino, 
                  Permutations_List *plist, double last_frame);
void draw_playfield (Playfield *playfield, int color_mode);
void draw_hud (Stats *stats, int color_mode, Permutations_List *plist);
Coord get_playfield_starting_yx (void);
void init_curses (void);
void uninit_curses (void);
int convert_input(int input);
double get_elapsed(struct timespec *start);
bool state_timer_reached (Stats *stats, struct timespec *state_timer);
void a_game_of_falling_blocks (int difficulty_level, int color_mode, int debug);

#endif

