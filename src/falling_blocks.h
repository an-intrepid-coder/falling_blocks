#ifndef FALLING_BLOCKS_H

#define FALLING_BLOCKS_H

#define GAME_SCREEN_HEIGHT 24
#define GAME_SCREEN_WIDTH 24
#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10
#define HUD_HEIGHT 24
#define HUD_WIDTH 14

bool game_fits_screen (void);
void get_game_screen_starting_yx (int *y, int *x);
void draw_playfield (int starting_y, int starting_x);
void draw_hud (int starting_y, int starting_x);
void draw_game_screen (void);

#endif
