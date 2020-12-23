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
typedef enum {SCORE_LINE, LEVEL_LINE, TICKS_LINE, STEP_LINE} Hud_Lines;

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

// Color modes
typedef enum {MODE_NO_COLOR, MODE_ASCII_COLOR, MODE_SOLID_COLOR} Color_Modes;

bool game_fits_screen ();
void pause_game ();
void draw_game (Playfield *playfield, Stats *stats, int color_mode);
void draw_playfield (Playfield *playfield, int mode);
Coord get_playfield_starting_yx (void);
void init_curses (void);
void uninit_curses (void);
int convert_input(int input);
double get_elapsed(struct timespec *start);
bool state_timer_reached (Stats *stats, struct timespec *state_timer);
void a_game_of_falling_blocks (int difficulty_level, int color_mode);

#endif

