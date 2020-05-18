#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "coord.c"

/* TO-DO:

    2. String parsing.

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

Coord get_game_screen_starting_yx () {
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


int main (int argc, char *argv[]) {

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    bool playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH] = {false};
    Stats stats;
    stats.name = "Player";
    stats.score = 0;
    stats.level = 1;
    // To-Do: Main loop w/ animated frames.

    Coord playfield_yx_start = get_game_screen_starting_yx();
    //Tetromino testing = tetromino_constructor(SQUARE, playfield_yx_start.x + 2);
    //Tetromino testing = tetromino_constructor(STRAIGHT, playfield_yx_start.x + 2);
    //Tetromino testing = tetromino_constructor(SKEW_A, playfield_yx_start.x + 2);
    //Tetromino testing = tetromino_constructor(SKEW_B, playfield_yx_start.x + 2);
    Tetromino testing = tetromino_constructor(L_A, playfield_yx_start.x + 2);
    //Tetromino testing = tetromino_constructor(L_B, playfield_yx_start.x + 2);
    //Tetromino testing = tetromino_constructor(T, playfield_yx_start.x + 2);

    draw_game_screen(playfield, &stats);

    //draw test tetromino
    for (int block = 0; block < NUM_BLOCKS; block++) {
        mvaddch(testing.blocks_yx[block].y, testing.blocks_yx[block].x, '#');
    }

    getch();

    echo();
    nocbreak();
    curs_set(1);
    endwin();

    return 0;    
}
