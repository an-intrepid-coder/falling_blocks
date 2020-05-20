#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "coord.c"

/* TO-DO:

    1. String parsing.

*/

bool game_fits_screen (void) {
    /* Returns true if the current terminal can fit the game screen.  */
    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    if (terminal_height >= GAME_SCREEN_HEIGHT && terminal_width >= GAME_SCREEN_WIDTH) {
        return true;
    } 
    return false;
}

Coord get_playfield_starting_yx (void) {
    /* takes pointers to two integers and returns the coordinates as an object.  */
    Coord new_coord;

    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    new_coord.y = (terminal_height / 2) - (GAME_SCREEN_HEIGHT / 2);
    new_coord.x = (terminal_width / 2) - (GAME_SCREEN_WIDTH / 2);

    return new_coord;
}

void draw_playfield (bool playfield[][PLAYFIELD_WIDTH]) {
    /* Takes the playfield array and the stats struct. Displays the results using
     * ncurses.  */
    Coord starting_yx = get_playfield_starting_yx();
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            if (playfield[y][x]) {
                mvaddch(starting_yx.y + y, starting_yx.x + x, '#');
            } else {
                mvaddch(starting_yx.y + y, starting_yx.x + x, '.');
            }
        }
    }
}

// A couple of curses wrappers
void init_curses (void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
}

void uninit_curses (void) {
    echo();
    nocbreak();
    curs_set(1);
    endwin();
}

int main (int argc, char *argv[]) {

    srand((unsigned) time(NULL));

    init_curses();

    // true = filled cell | false = empty cell
    bool playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH] = {false};

    Stats stats;
    stats.name = "Player";
    stats.score = 0;
    stats.level = 1;
    stats.ticks = 0;
    // To-Do: Main loop w/ non-blocking I/O and constant framerate.

    for (;;) {
        // Test of tetromino objects
        int test_topleft_x = rand() % 7;
        int test_tetromino_type = rand() % 7;

        erase();

        Tetromino testing = tetromino_constructor(test_tetromino_type, test_topleft_x, 
                                                  playfield);

        for (;;) {
            draw_playfield(playfield);
            //draw_tetromino(playfield_yx_start, &testing);
            getch();
            stats.ticks += 1;
        }
    }

    // End test

    uninit_curses();

    return 0;    
}
