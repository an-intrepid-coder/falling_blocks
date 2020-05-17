#include <ncurses.h>
#include <stdbool.h>

#include "falling_blocks.h"

bool game_fits_screen (void) {
    /* Returns true if the current terminal can fit the game screen.  */
    int terminal_height, terminal_width;

    getmaxyx(stdscr, terminal_height, terminal_width);

    if (terminal_height >= GAME_SCREEN_HEIGHT && terminal_width >= GAME_SCREEN_WIDTH) {
        return true;
    } 
    
    return false;
}

void get_game_screen_starting_yx (int *y, int *x) {
    /* takes pointers to two integers and stores the starting coordinates for
     * the game screen.  */
    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    *y = (terminal_height / 2) - (GAME_SCREEN_HEIGHT / 2);
    *x = (terminal_width / 2) - (GAME_SCREEN_WIDTH / 2);
}

void draw_playfield (int starting_y, int starting_x) {
    /* draws the portion of the game screen where the blocks fall.  */
    for (int line = starting_y; line < starting_y + PLAYFIELD_HEIGHT; line++) {
        for (int cell = starting_x; cell < starting_x + PLAYFIELD_WIDTH; cell++) {
            mvaddch(line, cell, 46);
        }
    }
}

void draw_hud (int starting_y, int starting_x) {
    /* draws the hud  */
    int hud_starting_x = starting_x + PLAYFIELD_WIDTH;
    for (int line = starting_y; line < starting_y + HUD_HEIGHT; line++) {
        for (int cell = hud_starting_x; cell < hud_starting_x + HUD_WIDTH; cell++) {
            mvaddch(line, cell, 72);
        }
    }
}

void draw_game_screen (void) {
    /* draws the playfield and the hud, centered to the terminal, if it fits.  */
    erase();

    if (game_fits_screen()) {
        int game_screen_starting_column, game_screen_starting_row;
        get_game_screen_starting_yx(&game_screen_starting_row, &game_screen_starting_column);
        draw_playfield(game_screen_starting_row, game_screen_starting_column);
        draw_hud(game_screen_starting_row, game_screen_starting_column);
    } else {
        addstr("Window must be at least 24x24.");
    }
}

int main (int argc, char *argv[]) {

    initscr();
    noecho();
    cbreak();

    draw_game_screen();
    getch();

    echo();
    nocbreak();
    endwin();

    return 0;    
}
