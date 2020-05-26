#ifndef FALLING_BLOCKS_H
#define FALLING_BLOCKS_H

#define GAME_SCREEN_HEIGHT 24
#define GAME_SCREEN_WIDTH 24
#define HUD_HEIGHT 24
#define HUD_WIDTH 14

#define FRAME_INTERVAL_IN_SECONDS (double) .1
#define USER_INPUT_INTERVAL_IN_SECONDS (double) .2
#define SPLASH_INTERVAL_IN_SECONDS (double) 3

#include <time.h>
#include "coord.h"
#include "tetromino.h"
#include "playfield.h"
#include "stats.h"

// Tetromino input enum 
typedef enum {DIRECTION_NONE, DOWN, LEFT, RIGHT, ROTATE} Input_Types;
// Clock types
typedef enum {FPS_CLOCK, STEP_CLOCK, USER_INPUT_CLOCK, SPLASH_CLOCK} Clock_Types;
// Splash flags
typedef enum {FLAG_NONE, LEVEL_UP} Splash_Flags;

bool game_fits_screen ();
void draw_playfield (Playfield *playfield);
Coord get_playfield_starting_yx (void);
void init_curses (void);
void uninit_curses (void);
int convert_input(int input);
bool timer_reached (clock_t clock_last, int clock_type, Stats *stats);

#endif
