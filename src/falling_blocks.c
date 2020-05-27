#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "coord.c"
#include "playfield.c"
#include "stats.c"

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
    /* Centers and displays the playfield.  */
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

void draw_hud (Stats *stats) {
    /* Centers and displays the HUD.  */
    Coord starting_yx = get_playfield_starting_yx();
    int starting_x = starting_yx.x + PLAYFIELD_WIDTH;

    mvprintw(starting_yx.y + SCORE_LINE, starting_x, "Score: %d", stats->score);
    mvprintw(starting_yx.y + LEVEL_LINE, starting_x, "Level: %d", stats->level);
    mvprintw(starting_yx.y + TICKS_LINE, starting_x, "Ticks: %d", stats->ticks);
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
    /* Is passed the ASCII value of the user input and returns an enum.  */
    int return_value = DIRECTION_NONE;
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

bool timer_reached (clock_t clock_last, int input_clock_type, Stats *stats) {
    /* Checks the different clocks used to keep the game in-sync and returns
     * true or false if the indicated timer is reached.  */
    bool return_value = false;
    double time_difference = (clock() - clock_last) / (double) CLOCKS_PER_SEC;
    switch (input_clock_type) {
        case FPS_CLOCK:
            if (time_difference > FRAME_INTERVAL_IN_SECONDS) {
                return_value = true;
            }
        break;
        case STEP_CLOCK:
            if (time_difference > stats->step_interval) {
                return_value = true;
            }
        break;
        case USER_INPUT_CLOCK:
            if (time_difference > USER_INPUT_INTERVAL_IN_SECONDS) {
                return_value = true;
            }
        break;
    }
    return return_value;
}

void game_over () {
    /* Turns off non-blocking mode and alerts the player to exit the game.  */
    nodelay(stdscr, false);
    mvprintw(0, 0, "Game Over!\n ... 'q' key to exit.");
    while (getch() != 113) {}
}

void a_game_of_falling_blocks (void) {
    clock_t fps_clock_last = clock();
    clock_t step_clock_last = clock();
    clock_t user_input_clock_last = clock();

    srand((unsigned) time(NULL));

    Playfield playfield = playfield_constructor();
    Stats stats = stats_constructor();

    int tetromino_topleft_x = rand() % 7;
    int tetromino_type = rand() % 7;

    Tetromino tetromino = tetromino_constructor(tetromino_type, tetromino_topleft_x, 
                                                &playfield);

    while (!tetromino.game_over) {
        int input = getch();
        if (timer_reached(fps_clock_last, FPS_CLOCK, &stats)){
            erase();
            draw_playfield(&playfield);
            draw_hud(&stats);
            fps_clock_last = clock();

        }
        if (timer_reached(step_clock_last, STEP_CLOCK, &stats)) {
            tetromino_move(&tetromino, &playfield, DOWN);
            step_clock_last = clock();
            stats_tick(&stats);
        } else if (input != ERR) {
            if (timer_reached(user_input_clock_last, USER_INPUT_CLOCK, &stats)) {
                tetromino_move(&tetromino, &playfield, convert_input(input));
                user_input_clock_last = clock();
            }
        }
        if (!tetromino_can_move(&tetromino, &playfield, DOWN)) {
            tetromino_freeze(&tetromino, &playfield);
            playfield_clear_lines(&playfield, &stats);
            tetromino_topleft_x = rand() % SPAWN_LIMIT;
            tetromino_type = rand() % SPAWN_LIMIT;
            tetromino = tetromino_constructor(tetromino_type, tetromino_topleft_x,
                                              &playfield);
            int spawn_rotations = rand() % SPAWN_LIMIT;
            for (int rotations = 0; rotations < spawn_rotations; rotations++) {
                tetromino_move(&tetromino, &playfield, ROTATE);
            }
        }
        if (stats_can_level_up(&stats)) {
            stats_level_up(&stats);
        }
    }
}

int main (int argc, char *argv[]) {

    init_curses();
    
    a_game_of_falling_blocks();
    
    game_over();

    uninit_curses();

    return EXIT_SUCCESS;
}
