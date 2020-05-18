#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

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

Coord get_game_screen_starting_yx (void) {
    /* takes pointers to two integers and returns the coordinates as an object.  */
    Coord new_coord;

    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    new_coord.y = (terminal_height / 2) - (GAME_SCREEN_HEIGHT / 2);
    new_coord.x = (terminal_width / 2) - (GAME_SCREEN_WIDTH / 2);

    return new_coord;
}

void draw_game_screen (bool playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH], Stats *stats) {
    /* Takes the playfield array and the stats struct. Displays the results using
     * ncurses.  */
    Coord starting_yx = get_game_screen_starting_yx();
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            if (playfield[y][x]) {
                mvaddch(starting_yx.y + y, starting_yx.x + x, '#');
            } else {
                mvaddch(starting_yx.y + y, starting_yx.x + x, '.');
            }
        }
    }

    int hud_x = starting_yx.x + (PLAYFIELD_WIDTH + 2);
    char label_score[] = "Score: ";
    char score_placeholder[] = "99"; // Replace with function shortly
    char label_level[] = "Level: ";
    char level_placeholder[] = "1"; // Replace with same function as above shortly

    mvaddstr(starting_yx.y, hud_x, stats->name);
    mvaddstr(starting_yx.y + 1, hud_x, label_score);
    addstr(score_placeholder);
    mvaddstr(starting_yx.y + 2, hud_x, label_level);
    addstr(level_placeholder);
}

void draw_tetromino(Coord playfield_yx_start, Tetromino *tetromino) {
    /* Takes the Coordinates of the top-left corner of the playfield
     * and a tetromino object. Draws the tetromino with ncurses.  */
    for (int block = 0; block < NUM_BLOCKS; block++) {
        mvaddch(playfield_yx_start.y + tetromino->blocks_yx[block].y, 
                tetromino->blocks_yx[block].x, '#');
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

    init_curses();

    bool playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH] = {false};
    Stats stats;
    stats.name = "Player";
    stats.score = 0;
    stats.level = 1;
    // To-Do: Main loop w/ animated frames.
    // To-Do: Drop zone and falling tetrominos.

    // Test of tetromino objects
    Coord playfield_yx_start = get_game_screen_starting_yx();
    int test_topleft_x = playfield_yx_start.x + 2;
    for (int test = 0; test < 7; test++) {
        erase();

        Tetromino testing = tetromino_constructor(test, test_topleft_x);

        draw_game_screen(playfield, &stats);

        draw_tetromino(playfield_yx_start, &testing);

        getch();
    }
    // End test

    uninit_curses();

    return 0;    
}
