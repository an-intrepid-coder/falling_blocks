#ifndef FALLING_BLOCKS_H
#define FALLING_BLOCKS_H

#define GAME_SCREEN_HEIGHT 24
#define GAME_SCREEN_WIDTH 24
#define HUD_HEIGHT 24
#define HUD_WIDTH 14
#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

typedef struct {
    /* A struct to keep track of game statistics. Passes some to the hud display.  */
    char *name;
    long long int score;
    int level; 
} Stats;

bool game_fits_screen ();
void draw_game_screen (bool playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH], Stats *stats);

#endif
