#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "coord.c"
#include "playfield.c"

/* TO-DO:

    1. String parsing for labels.

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

void draw_playfield (Playfield *playfield) {
    /* Takes the playfield array and the stats struct. Displays the results using
     * ncurses.  */
    Coord starting_yx = get_playfield_starting_yx();
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            if (playfield->cell_filled[y][x]) {
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

int get_mvdir (void) {
    /* Reads user input and returns an enum.  */
    int input = getch();
    switch (input) {
        case 83:
        case 115:
            return DOWN;
        break;
        case 65:
        case 97:
            return LEFT;
        break;
        case 68:
        case 100:
            return RIGHT;
        break;
    }
    return NONE;
}

int main (int argc, char *argv[]) {

    srand((unsigned) time(NULL));

    init_curses();

    Playfield playfield = playfield_constructor();

    // To-Do: Main loop w/ non-blocking I/O and constant framerate.

    for (;;) {
        // Test of tetromino objects
        int test_topleft_x = rand() % 7;
        int test_tetromino_type = rand() % 7;

        Tetromino testing = tetromino_constructor(test_tetromino_type, test_topleft_x, 
                                                  &playfield);

        for (;;) {
            erase();
            draw_playfield(&playfield);
            tetromino_move(&testing, &playfield, get_mvdir());
            if (!tetromino_can_move(&testing, &playfield, DOWN)) {
                tetromino_freeze(&testing, &playfield);
                test_topleft_x = rand() % 7;
                test_tetromino_type = rand() % 7;
                testing = tetromino_constructor(test_tetromino_type, test_topleft_x,
                                                &playfield);
            }
        }
    }

    // End test

    uninit_curses();

    return 0;    
}
