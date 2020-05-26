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
    nodelay(stdscr, true);
}

void uninit_curses (void) {
    echo();
    nocbreak();
    curs_set(1);
    nodelay(stdscr, false);
    endwin();
}

int convert_input (int input) {
    /* Reads user input and returns an enum.  */
    int return_value = NONE;
    switch (input) {
        case 83:
        case 115:
            return_value = DOWN;
        break;
        case 65:
        case 97:
            return_value = LEFT;
        break;
        case 68:
        case 100:
            return_value = RIGHT;
        break;
        default:
            return_value = ROTATE;
        break;
    }
    return return_value;
}

bool timer_reached (clock_t clock_last, int input_clock_type) {
    bool return_value = false;
    double time_difference = (clock() - clock_last) / (double) CLOCKS_PER_SEC;
    switch (input_clock_type) {
        case FPS_CLOCK:
            if (time_difference > FRAME_INTERVAL_IN_SECONDS) {
                return_value = true;
            }
        break;
        case STEP_CLOCK:
            if (time_difference > STEP_INTERVAL_IN_SECONDS) {
                return_value = true;
            }
        break;
        case USER_INPUT_CLOCK:
            if (time_difference > USER_INPUT_INTERVAL_IN_SECONDS) {
                return_value = true;
            }
    }
    return return_value;
}

int main (int argc, char *argv[]) {

    clock_t fps_clock_last = clock();
    clock_t step_clock_last = clock();
    clock_t user_input_clock_last = clock();
    clock_t splash_clock_last = clock();

    srand((unsigned) time(NULL));

    init_curses();

    Playfield playfield = playfield_constructor();

    for (;;) {
        int tetromino_topleft_x = rand() % 7;
        int tetromino_type = rand() % 7;

        Tetromino tetromino = tetromino_constructor(tetromino_type, tetromino_topleft_x, 
                                                    &playfield);

        for (;;) {
            int input = getch();
            if (timer_reached(fps_clock_last, FPS_CLOCK)){
                erase();
                draw_playfield(&playfield);
                fps_clock_last = clock();
            }
            if (timer_reached(step_clock_last, STEP_CLOCK)) {
                tetromino_move(&tetromino, &playfield, DOWN);
                step_clock_last = clock();
            } else if (input != ERR) {
                if (timer_reached(user_input_clock_last, USER_INPUT_CLOCK)) {
                    tetromino_move(&tetromino, &playfield, convert_input(input));
                    user_input_clock_last = clock();
                }
            }
            
            if (!tetromino_can_move(&tetromino, &playfield, DOWN)) {
                tetromino_freeze(&tetromino, &playfield);
                playfield_clear_lines(&playfield);
                tetromino_topleft_x = rand() % 7;
                tetromino_type = rand() % 7;
                tetromino = tetromino_constructor(tetromino_type, tetromino_topleft_x,
                                                  &playfield);
            }
        }
    }

    uninit_curses();

    return 0;    
}
