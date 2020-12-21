#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "playfield.c"
#include "stats.c"

bool game_fits_screen (void) {
    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    if (terminal_height >= GAME_SCREEN_HEIGHT && terminal_width >= GAME_SCREEN_WIDTH) {
        return true;
    } 
    return false;
}

Coord get_playfield_starting_yx (void) {
    Coord new_coord;

    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    new_coord.y = (terminal_height / 2) - (GAME_SCREEN_HEIGHT / 2);
    new_coord.x = (terminal_width / 2) - (GAME_SCREEN_WIDTH / 2);

    return new_coord;
}

void draw_playfield (Playfield *playfield, int mode) {
    /* Centers and displays the playfield. This is a naive version and needs serious
     * optimization.  */
    Coord starting_yx = get_playfield_starting_yx();
    for (int row = 0; row < PLAYFIELD_HEIGHT; row++) {
        for (int cell = 0; cell < PLAYFIELD_WIDTH; cell++) {
            switch (mode) {
                case MODE_ASCII_COLOR:
                    switch (playfield->fill_type[row][cell]) {
                        case FILL_EMPTY:
                            attron(COLOR_PAIR(PAIR_NORMAL));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '.');
                            attroff(COLOR_PAIR(PAIR_NORMAL));
                        break;
                        case FILL_SQUARE:
                            attron(COLOR_PAIR(PAIR_SQUARE_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_SQUARE_ASCII));
                        break;
                        case FILL_STRAIGHT:
                            attron(COLOR_PAIR(PAIR_STRAIGHT_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_STRAIGHT_ASCII));
                        break;
                        case FILL_SKEW_A:
                            attron(COLOR_PAIR(PAIR_SKEW_A_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_SKEW_A_ASCII));
                        break;
                        case FILL_SKEW_B:
                            attron(COLOR_PAIR(PAIR_SKEW_B_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_SKEW_B_ASCII));
                        break;
                        case FILL_L_A:
                            attron(COLOR_PAIR(PAIR_L_A_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_L_A_ASCII));
                        break;
                        case FILL_L_B:
                            attron(COLOR_PAIR(PAIR_L_B_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_L_B_ASCII));
                        break;
                        case FILL_T:
                            attron(COLOR_PAIR(PAIR_T_ASCII));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                            attroff(COLOR_PAIR(PAIR_T_ASCII));
                        break;
                    }
                break;
                case MODE_SOLID_COLOR:
                    switch (playfield->fill_type[row][cell]) {
                        case FILL_EMPTY:
                            attron(COLOR_PAIR(PAIR_NORMAL));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, '.');
                            attroff(COLOR_PAIR(PAIR_NORMAL));
                        break;
                        case FILL_SQUARE:
                            attron(COLOR_PAIR(PAIR_SQUARE_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_SQUARE_SOLID));
                        break;
                        case FILL_STRAIGHT:
                            attron(COLOR_PAIR(PAIR_STRAIGHT_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_STRAIGHT_SOLID));
                        break;
                        case FILL_SKEW_A:
                            attron(COLOR_PAIR(PAIR_SKEW_A_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_SKEW_A_SOLID));
                        break;
                        case FILL_SKEW_B:
                            attron(COLOR_PAIR(PAIR_SKEW_B_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_SKEW_B_SOLID));
                        break;
                        case FILL_L_A:
                            attron(COLOR_PAIR(PAIR_L_A_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_L_A_SOLID));
                        break;
                        case FILL_L_B:
                            attron(COLOR_PAIR(PAIR_L_B_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_L_B_SOLID));
                        break;
                        case FILL_T:
                            attron(COLOR_PAIR(PAIR_T_SOLID));
                            mvaddch(starting_yx.y + row, starting_yx.x + cell, ' ');
                            attroff(COLOR_PAIR(PAIR_T_SOLID));
                        break;
                    }   
                break;
                case MODE_NO_COLOR:
                    if (playfield->fill_type[row][cell]) {
                        mvaddch(starting_yx.y + row, starting_yx.x + cell, '#');
                    } else {
                        mvaddch(starting_yx.y + row, starting_yx.x + cell, '.');
                    }
                break;
            }
        }
    }
}

void draw_hud (Stats *stats) {
    Coord starting_yx = get_playfield_starting_yx();
    int starting_x = starting_yx.x + PLAYFIELD_WIDTH;

    mvprintw(starting_yx.y + SCORE_LINE, starting_x, "Score: %d", stats->score);
    mvprintw(starting_yx.y + LEVEL_LINE, starting_x, "Level: %d", stats->level);
    mvprintw(starting_yx.y + TICKS_LINE, starting_x, "Ticks: %d", stats->ticks);
    mvprintw(starting_yx.y + STEP_LINE, starting_x, "Step Interval: %f seconds",
             stats->step_interval);
}

void draw_game (Playfield *playfield, Stats *stats, int color_mode) {
    static bool init = true;
    static int last_rows, last_cols;
    int rows, cols;

    erase(); 

    if (init) {
        getmaxyx(stdscr, last_rows, last_cols);
        init = false;
    } 

    getmaxyx(stdscr, rows, cols);

    if (rows != last_rows || cols != last_cols) {
        getmaxyx(stdscr, last_rows, last_cols);
        erase();
    }

    draw_playfield(playfield, color_mode);
    draw_hud(stats);
}

// A couple of curses wrappers
void init_curses (void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    if (has_colors()) {
        start_color();
        init_pair(PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_SQUARE_ASCII, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_STRAIGHT_ASCII, COLOR_CYAN, COLOR_BLACK);
        init_pair(PAIR_SKEW_A_ASCII, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_SKEW_B_ASCII, COLOR_BLUE, COLOR_BLACK);
        init_pair(PAIR_L_A_ASCII, COLOR_GREEN, COLOR_BLACK);
        init_pair(PAIR_L_B_ASCII, COLOR_YELLOW, COLOR_BLACK);
        init_pair(PAIR_T_ASCII, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(PAIR_SQUARE_SOLID, COLOR_WHITE, COLOR_WHITE);
        init_pair(PAIR_STRAIGHT_SOLID, COLOR_CYAN, COLOR_CYAN);
        init_pair(PAIR_SKEW_A_SOLID, COLOR_RED, COLOR_RED);
        init_pair(PAIR_SKEW_B_SOLID, COLOR_BLUE, COLOR_BLUE);
        init_pair(PAIR_L_A_SOLID, COLOR_GREEN, COLOR_GREEN);
        init_pair(PAIR_L_B_SOLID, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(PAIR_T_SOLID, COLOR_MAGENTA, COLOR_MAGENTA);
    }
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
        case 70:
        case 102:
            return_value = ROTATE;
        break;
        case 80:
        case 112:
            return_value = PAUSE;
        break;
        default:
            return_value = DIRECTION_NONE;
        break;
    }
    return return_value;
}

double get_elapsed (struct timespec *start) {
    struct timespec end;

    clock_gettime(CLOCK_REALTIME, &end);

    double start_secs, end_secs;
    start_secs = start->tv_sec + start->tv_nsec / (double) NSECS_PER_SEC;
    end_secs = end.tv_sec + end.tv_nsec / (double) NSECS_PER_SEC;

    return end_secs - start_secs;
}

bool state_timer_reached (Stats *stats, struct timespec *state_timer) {

    double elapsed = get_elapsed(state_timer);

    if (elapsed >= stats->step_interval) {
        clock_gettime(CLOCK_REALTIME, state_timer);
        return true;
    }

    return false;
}

void frame_wait(struct timespec *loop_timer) {
    double delay = get_elapsed(loop_timer);

    struct timespec wait = {
        0,
        (long int) (FRAME_INTERVAL - delay) * NSECS_PER_SEC,
    };

    nanosleep(&wait, NULL); 
}

void game_over () {
    /* Turns off non-blocking mode and alerts the player to exit the game.  */
    nodelay(stdscr, false);
    mvprintw(0, 0, "Game Over!");
    mvprintw(1, 0, " ... 'q' key to exit.");
    while (getch() != 113) {}
}

void pause_game (Stats *stats) {
    nodelay(stdscr, false);
    mvprintw(0, 0, "GAME PAUSED");
    mvprintw(1, 0, "...any key to continue...");
    getch();
    stats->paused = false;
    erase();
    nodelay(stdscr, true);
}

void a_game_of_falling_blocks (int difficulty_level, int color_mode) {
    srand((unsigned) time(NULL));

    Playfield playfield = playfield_constructor();
    Stats stats = stats_constructor();

    int tetromino_topleft_x = rand() % SPAWN_LIMIT;
    int tetromino_type = rand() % SPAWN_LIMIT;

    Tetromino tetromino = tetromino_constructor(tetromino_type, tetromino_topleft_x, 
                                                &playfield);

    struct timespec state_timer, loop_timer;
    clock_gettime(CLOCK_REALTIME, &state_timer);
    while (!tetromino.game_over) {
        int input;
        clock_gettime(CLOCK_REALTIME, &loop_timer);
 
        if (state_timer_reached(&stats, &state_timer)) {
            tetromino_move(&tetromino, &playfield, DOWN);
            stats_tick(&stats);
        } else if ((input = getch()) != ERR) {
            if (convert_input(input) == PAUSE) {
                stats.paused = true;
            } else {
                tetromino_move(&tetromino, &playfield, convert_input(input));
            }
            while ((getch() != ERR)){}
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
            stats_level_up(&stats, difficulty_level);
        }
    
        if (stats.paused) {
            pause_game(&stats);
        }

        draw_game(&playfield, &stats, color_mode);

        frame_wait(&loop_timer);
    }
}

int main (int argc, char *argv[]) {

    int difficulty_level = DIFFICULTY_MEDIUM;
    int color_mode = MODE_NO_COLOR;

    char *difficulty_levels[] = {
        "-easy", "-medium", "-hard"
    };
    char *color_modes[] = {
        "-nocolor", "-ascii", "-solid"
    };

    for (int arg = 1; arg < argc; arg++) {
        if (strcmp(argv[arg], difficulty_levels[0]) == 0) {
            difficulty_level = DIFFICULTY_EASY;
        } else if (strcmp(argv[arg], difficulty_levels[2]) == 0) {
            difficulty_level = DIFFICULTY_HARD;
        } else if (strcmp(argv[arg], color_modes[1]) == 0) {
            color_mode = MODE_ASCII_COLOR;
        } else if (strcmp(argv[arg], color_modes[2]) == 0) {
            color_mode = MODE_SOLID_COLOR;
        }
    }

    init_curses();
   
    a_game_of_falling_blocks(difficulty_level, color_mode);
    
    game_over();

    uninit_curses();

    return EXIT_SUCCESS;
}

