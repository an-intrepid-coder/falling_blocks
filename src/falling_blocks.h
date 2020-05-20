#ifndef FALLING_BLOCKS_H
#define FALLING_BLOCKS_H

#define GAME_SCREEN_HEIGHT 24
#define GAME_SCREEN_WIDTH 24
#define HUD_HEIGHT 24
#define HUD_WIDTH 14

#define NUM_MAX_TETROMINOS 100

#include "coord.h"
#include "tetromino.h"
#include "playfield.h"

typedef enum {DOWN, LEFT, RIGHT} Movement_Dirs;

typedef struct {
    /* A struct to keep track of game statistics. Passes some to the hud display.  */
    char *name;
    long long int score;
    long long int ticks;
    int level; 
} Stats;

bool game_fits_screen ();
void draw_game_screen (bool playfield[][PLAYFIELD_WIDTH], Stats *stats);
Coord get_playfield_starting_yx (void);
void init_curses (void);
void uninit_curses (void);

#endif
